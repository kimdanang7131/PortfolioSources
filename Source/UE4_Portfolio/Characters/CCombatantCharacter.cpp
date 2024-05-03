#include "Characters/CCombatantCharacter.h"
#include "Global.h"

//////////////////

#include "Components/CWeaponStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CSkillComponent.h"

//////////////////

#include "Action/CMontageDataAsset.h"
#include "Action/CWeapon.h"

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

/** �ǰ� ������ �� */
float ACCombatantCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageInstigator = EventInstigator;
	DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	// �������� LaunchAmount�� �����ͼ� �� �縸ŭ ���ư��� ����������� ( ���� �� ���� ����Ǿ )
	ACCombatantCharacter* CombatCharacter = Cast<ACCombatantCharacter>(EventInstigator->GetPawn());
	if (CombatCharacter)
		SetLaunchAmount(CombatCharacter->GetLaunchAmount());

	State->SetHittedMode();

	return Status->GetHealth();
}


void ACCombatantCharacter::ToggleWeaponA()
{
	WeaponState->ToggleWeaponA();
}

void ACCombatantCharacter::ToggleWeaponB()
{
	WeaponState->ToggleWeaponB();
}


/** �ǰ� ������ ��*/
void ACCombatantCharacter::Hitted()
{
	Super::Hitted();

	// #1. StatusComponent�� ������ �� �����ϰ� 0���� ������ DeadMode
	Status->SubHealth(DamageValue);
	if (Status->GetHealth() - DamageValue <= 0.0f)
	{
		DamageValue = 0.0f;
		State->SetDeadMode();
		return;
	}

	DamageValue = 0.0f;

	// #2. ���� DataAsset�� ��ϵ� �ִϸ��̼� ��������
	UCMontageDataAsset** MontageData = StateMontageMap.Find(State->GetStateNow());
	FALSE_RETURN(*MontageData);

	// #3. SetStop�� ���� Key Input�� Ignore ��Ŵ
	Status->SetStop();
	PlayAnimMontage((*MontageData)->GetMontageData(0).AnimMontage, (*MontageData)->GetMontageData(0).PlayRatio, (*MontageData)->GetMontageData(0).StartSection);

	// #4. �¾��� �� �������� �Ĵٺ��� ����
	FVector start = GetActorLocation();
	FVector target = DamageInstigator->GetPawn()->GetActorLocation();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
	DamageInstigator = NULL;

	// #5. ���������κ��� ���� ������ Launch ��Ŵ
	FVector direction = start - target;
	direction.Normalize();
	direction.Z = 0.f;

	LaunchCharacter(direction * launchAmount, true, false);

	// #6. ���ܻ�Ȳ) ���������ϴٰ� �¾��� �� Action Setting �ʱ�ȭ��
	if (WeaponState)
	{
		if (WeaponState->GetCurrentWeapon())
			WeaponState->GetCurrentWeapon()->ResetAction();
	}
}

/** State DataAsset - ȸ�� ��� */
void ACCombatantCharacter::Dodging()
{
	FALSE_RETURN(State->IsIdleMode());
	State->SetDodgeMode();

	// #1. DataAsset�� �ִٸ� Dodge �ִϸ��̼� ��������
	UCMontageDataAsset** MontageData = StateMontageMap.Find(State->GetStateNow());
	FALSE_RETURN(*MontageData);
	
	// #2. ���� �ֱٿ� �Է��� Vector �����ͼ� 0�̸� ���� ���ͷ� ������
	FVector LastVector = GetLastMovementInputVector();
	if (LastVector.IsNearlyZero())
		LastVector = CSub::GetCustomForwardVector(this);


	// #3. �������� �Է��� �������� ȸ�� ���� ��Ű�� ���� �������� ������ �ִϸ��̼� ����
	FRotator DodgeDirection = UKismetMathLibrary::MakeRotFromX(LastVector);
	SetActorRotation(DodgeDirection);

	PlayAnimMontage((*MontageData)->GetMontageData(0).AnimMontage , (*MontageData)->GetMontageData(0).PlayRatio, (*MontageData)->GetMontageData(0).StartSection);
}

