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

	/** ĳ���� �⺻ ���� ����*/
	{
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

		GetMesh()->SetRelativeLocation(FVector(0, 0, -90.f));
		GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));

		/** �ִϸ��̼� Pre���� */
		TSubclassOf<UAnimInstance> animInstance;
		CHelpers::MyFClassFinder<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Blueprints/Characters/ABP_CCharacter.ABP_CCharacter_C'");
		GetMesh()->SetAnimInstanceClass(animInstance);

		/** Mesh Pre���� */
		USkeletalMesh* mesh;
		CHelpers::MyFObjectFinder<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Meshes/UE4_Mannequin/SK_Mannequin.SK_Mannequin'");
		GetMesh()->SetSkeletalMesh(mesh);
	}

	/** nullptr�� ������� BP���� DataAsset�� �����Ͽ� ������ �ֵ��� */
	for (int32 i = 0; i < (int32)EStateType::Max; ++i)
		StateMontageMap.Add((EStateType)i, nullptr); 
}

void ACCharacter::BeginPlay()
{
	Super::BeginPlay();

	// #1. State�� ����ɶ����� DataAsset(Stata)�� ���� Montage�� ����ǵ���
	State->OnStateTypeChanged.AddDynamic(this, &ACCharacter::OnStateTypeChanged);

	// #2. ĳ���� �ʱ� ����
	Status->SetMove();
	Status->SetControl();
	State->SetIdleMode();
}

void ACCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


/** StateComponent�� ���� State�� �ٲ𶧸��� ����� Delegate�� ���� Character���� �Լ��� ���� */
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



/** Dead�� �� Destroyed���� �ڽĵ� ��� �ı��� ��ü �ı� */
void ACCharacter::Destroyed()
{
	Super::Destroyed();

	// #1. �� ���Ϳ� ���� ���͸� ��������
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	
	// #2. ���� ���͵� ��� �ı�
	for (int32 i = 0; i < AttachedActors.Num(); i++)
		AttachedActors[i]->Destroy();

	// #3. ���� ��ü(this) �ı��Ǵµ�
}

/** �׾��� ���� ����*/
void ACCharacter::Dead()
{
	// #1. ���̻� ���ݹ��� �ʰ� -> ��� SetCanDamaged�� �±���
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	// #2. State DataAsset�� ��ϵ� �ִϸ��̼� ����
	UCMontageDataAsset** MontageData = StateMontageMap.Find(State->GetStateNow());
	FALSE_RETURN(*MontageData);
	PlayAnimMontage((*MontageData)->GetMontageData(0).AnimMontage, (*MontageData)->GetMontageData(0).PlayRatio, (*MontageData)->GetMontageData(0).StartSection);
}






