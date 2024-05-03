#include "Characters/CCharacter.h"
#include "Global.h"

/////////////

#include "Components/CInventoryComponent.h"
#include "Components/CPostprocessComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"

#include "Action/CMontageDataAsset.h"
/////////////

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"


ACCharacter::ACCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCInventoryComponent>(this, &Inventory,"Inventory");
	CHelpers::CreateActorComponent<UCPostprocessComponent>(this, &Postprocess, "Postprocess");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");

	/** 캐릭터 기본 설정 모음*/
	{
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

		GetMesh()->SetRelativeLocation(FVector(0, 0, -90.f));
		GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));

		/** 애니메이션 Pre설정 */
		TSubclassOf<UAnimInstance> animInstance;
		CHelpers::MyFClassFinder<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Blueprints/Characters/ABP_CCharacter.ABP_CCharacter_C'");
		GetMesh()->SetAnimInstanceClass(animInstance);

		/** Mesh Pre설정 */
		USkeletalMesh* mesh;
		CHelpers::MyFObjectFinder<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Meshes/UE4_Mannequin/SK_Mannequin.SK_Mannequin'");
		GetMesh()->SetSkeletalMesh(mesh);
	}

	/** nullptr로 열어놔서 BP에서 DataAsset을 선택하여 넣을수 있도록 */
	for (int32 i = 0; i < (int32)EStateType::Max; ++i)
		StateMontageMap.Add((EStateType)i, nullptr); 
}

void ACCharacter::BeginPlay()
{
	Super::BeginPlay();

	// #1. State가 변경될때마다 DataAsset(Stata)에 넣은 Montage가 실행되도록
	State->OnStateTypeChanged.AddDynamic(this, &ACCharacter::OnStateTypeChanged);

	// #2. 캐릭터 초기 설정
	Status->SetMove();
	Status->SetControl();
	State->SetIdleMode();
}

void ACCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


/** StateComponent를 통해 State가 바뀔때마다 연결된 Delegate를 통해 Character안의 함수를 실행 */
void ACCharacter::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Dodge:  Dodging();  break;
	case EStateType::Dead:   Dead();     break;
	case EStateType::Hitted: Hitted(); break;
	default:
		break;
	}
}



/** Dead할 시 Destroyed에서 자식들 모두 파괴후 본체 파괴 */
void ACCharacter::Destroyed()
{
	Super::Destroyed();

	// #1. 이 액터에 붙은 액터를 가져오기
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	
	// #2. 붙은 액터들 모두 파괴
	for (int32 i = 0; i < AttachedActors.Num(); i++)
		AttachedActors[i]->Destroy();

	// #3. 이후 본체(this) 파괴되는듯
}

/** 죽었을 때의 설정*/
void ACCharacter::Dead()
{
	// #1. 더이상 공격받지 않게 -> 사실 SetCanDamaged가 맞긴함
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	// #2. State DataAsset에 등록된 애니메이션 실행
	UCMontageDataAsset** MontageData = StateMontageMap.Find(State->GetStateNow());
	FALSE_RETURN(*MontageData);
	PlayAnimMontage((*MontageData)->GetMontageData(0).AnimMontage, (*MontageData)->GetMontageData(0).PlayRatio, (*MontageData)->GetMontageData(0).StartSection);
}






