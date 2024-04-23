#include "Characters/CCharacter.h"
#include "Global.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

#include "Components/CInventoryComponent.h"
#include "Components/CPostprocessComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"

#include "Action/CMontageDataAsset.h"


ACCharacter::ACCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCInventoryComponent>(this, &Inventory,"Inventory");
	CHelpers::CreateActorComponent<UCPostprocessComponent>(this, &Postprocess, "Postprocess");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");

	bUseControllerRotationYaw = false;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Blueprints/Characters/ABP_CCharacter.ABP_CCharacter_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Meshes/UE4_Mannequin/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);


	// 맵을 초기화
	for (int32 i = 0; i < (int32)EStateType::Max; ++i)
	{
		StateMontageMap.Add((EStateType)i, nullptr); // 모든 값은 일단 null로 설정
	}
}

void ACCharacter::BeginPlay()
{
	Super::BeginPlay();
	Status->SetMove();
	Status->SetControl();

	State->OnStateTypeChanged.AddDynamic(this, &ACCharacter::OnStateTypeChanged);
	State->SetIdleMode();
}

void ACCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACCharacter::Destroyed()
{
	Super::Destroyed();

	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	
	for (int32 i = 0; i < AttachedActors.Num(); i++)
	{
		CLog::Print(AttachedActors[i]->GetName());
		AttachedActors[i]->Destroy();
	}
}

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

void ACCharacter::Hitted()
{
	CLog::Print("Hitted");
	UCMontageDataAsset** MontageData = StateMontageMap.Find(State->GetStateNow());
	FALSE_RETURN(*MontageData);

	Status->SetStop();

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return;
	}

	PlayAnimMontage((*MontageData)->GetMontageData(0).AnimMontage, (*MontageData)->GetMontageData(0).PlayRatio, (*MontageData)->GetMontageData(0).StartSection);
}

void ACCharacter::Dead()
{
	CLog::Print("Dead");
	UCMontageDataAsset** MontageData = StateMontageMap.Find(State->GetStateNow());
	FALSE_RETURN(*MontageData);

	PlayAnimMontage((*MontageData)->GetMontageData(0).AnimMontage, (*MontageData)->GetMontageData(0).PlayRatio, (*MontageData)->GetMontageData(0).StartSection);
}

void ACCharacter::Dodging()
{
	UCMontageDataAsset** MontageData = StateMontageMap.Find(State->GetStateNow());
	FALSE_RETURN(*MontageData);

	//(*MontageData)->GetMontageData(0);
}

