#include "Characters/CCombatantCharacter.h"
#include "Global.h"

#include "Components/CSkillComponent.h"

#include "Components/CWeaponStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"

#include "Action/CMontageDataAsset.h"

ACCombatantCharacter::ACCombatantCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCSkillComponent>(this, &Skill, "Skill");
	CHelpers::CreateActorComponent<UCWeaponStateComponent>(this, &WeaponState, "WeaponState");
}

void ACCombatantCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ACCombatantCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACCombatantCharacter::OnWeaponStateTypeChanged(EWeaponStateType InPrevType, EWeaponStateType InNewType)
{
	//switch (InNewType)
	//{
	//    case EWeaponStateType::Unarmed:   WeaponState->SetUnarmedMode();   break;
	//    case EWeaponStateType::OneHand:   WeaponState->SetOneHandMode();   break;
	//    case EWeaponStateType::TwoHand:   WeaponState->SetTwoHandMode();   break;
	//    case EWeaponStateType::Katana:    WeaponState->SetKatanaMode();    break;
	//    case EWeaponStateType::DualBlade: WeaponState->SetDualBladeMode(); break;
	//}
}

float ACCombatantCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageInstigator = EventInstigator;
	DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	

	State->SetHittedMode();

	return Status->GetHealth();
}

void ACCombatantCharacter::ExecuteDodge()
{
	FALSE_RETURN(State->IsIdleMode());

	State->SetDodgeMode();
}

void ACCombatantCharacter::ToggleWeaponA()
{
	WeaponState->ToggleWeaponA();
}

void ACCombatantCharacter::ToggleWeaponB()
{
	WeaponState->ToggleWeaponB();
}

void ACCombatantCharacter::Dodging()
{
	UCMontageDataAsset** MontageData = StateMontageMap.Find(State->GetStateNow());
	FALSE_RETURN(*MontageData);

	FVector LastVector = GetLastMovementInputVector();

	if (LastVector.IsNearlyZero())
		LastVector = CSub::GetCustomForwardVector(this);


	FRotator DodgeDir = UKismetMathLibrary::MakeRotFromX(LastVector);
	SetActorRotation(DodgeDir);

	PlayAnimMontage((*MontageData)->GetMontageData(0).AnimMontage , (*MontageData)->GetMontageData(0).PlayRatio, (*MontageData)->GetMontageData(0).StartSection);
}

//void ACCombatantCharacter::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
//{
//	Super::OnStateTypeChanged(InPrevType, InNewType);
//}.


//Status->SubHealth(DamageValue);
//Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
//DamageValue = 0.0f;
//
//Status->SetStop();
//
//if (Status->GetHealth() <= 0.0f)
//{
//	State->SetDeadMode();
//
//	return;
//}
//
//
//
//Montages->PlayHitted();
//
//
//FVector start = GetActorLocation();
//FVector target = DamageInstigator->GetPawn()->GetActorLocation();
//SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
//DamageInstigator = NULL;
//
//FVector direction = target - start;
//direction.Normalize();
//LaunchCharacter(-direction * LaunchAmount, true, false);
//
//ChangeColor(FLinearColor(1, 0, 0, 1));
//
//UKismetSystemLibrary::K2_SetTimer(this, "RestoreColor", 0.1f, false);