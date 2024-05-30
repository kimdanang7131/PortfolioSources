#include "Characters/CCombatantCharacter.h"
#include "Global.h"
//////////////////
#include "Components/UIComponents/CSkillComponent.h"
#include "Components/CWeaponStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"
//////////////////
#include "Components/CapsuleComponent.h"
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
	
	// #1. �������� LaunchAmount�� �����ͼ� �� �縸ŭ ���ư��� ����������� ( ���� �� ���� ����Ǿ )
	ACCombatantCharacter* CombatantCharacter = Cast<ACCombatantCharacter>(EventInstigator->GetPawn());
	if (CombatantCharacter)
		SetLaunchAmount(CombatantCharacter->GetLaunchAmount());

	// #2. DataAsset�� ������ Hitted�� ����ǵ��� �ϱ�
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

/** �ǰ� ������ �� */
void ACCombatantCharacter::Hitted()
{
	Super::Hitted();

	FVector start = GetActorLocation();
	FVector target = DamageInstigator->GetPawn()->GetActorLocation();

	hitAngle = 0.f;
	bool bIsLeft = false;
	int32 animIndex = GetHitDirection(start, target, hitAngle, bIsLeft);

	// #1. StatusComponent�� ������ �� �����ϰ� 0���� ������ DeadMode �ٲٰ� return
	Status->SubHealth(DamageValue);
	DamageValue = 0.0f;

	
	if (State->IsDeadMode())
	{
		// �׾��� ���� ������ ���� �ٶ󺸴� ���� ����
		if (hitAngle <= 90.f)
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
		else
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(target, start));

		return;
	}

	//#2. ���� DataAsset�� ��ϵ� �ִϸ��̼� ��������
	UCMontageDataAsset** MontageData = StateMontageMap.Find(State->GetStateNow());
	FALSE_RETURN(*MontageData);

	//#3. SetStop�� ���� Key Input�� Ignore ��Ŵ
	Status->SetStop();
	PlayAnimMontage((*MontageData)->GetMontageData(animIndex).AnimMontage, (*MontageData)->GetMontageData(animIndex).PlayRatio, (*MontageData)->GetMontageData(animIndex).StartSection);
	//SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
	DamageInstigator = NULL;
	
	// #4. ���������κ��� ���� ������ Launch ��Ŵ
	FVector direction = start - target;
	direction.Normalize();
	direction.Z = 0.f;
	LaunchCharacter(direction * launchAmount, true, false);

	// #5. ���ܻ�Ȳ) ���������ϴٰ� �¾��� �� Action Setting �ʱ�ȭ��
	if (WeaponState)
	{
		if (WeaponState->GetCurrentWeapon())
			WeaponState->GetCurrentWeapon()->ResetAction();
	}
}

void ACCombatantCharacter::Dead()
{
	// #1. �տ��� �¾Ҵ��� �ڿ��� �¾Ҵ���
	int32 animIndex = hitAngle <= 90 ? 0 : 1;

	// #2. ���̻� ���ݹ��� �ʰ� -> ��� SetCanDamaged�� �±���
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// #3. State DataAsset�� ��ϵ� �ִϸ��̼� ����
	UCMontageDataAsset** MontageData = StateMontageMap.Find(State->GetStateNow());
	FALSE_RETURN(*MontageData);
	PlayAnimMontage((*MontageData)->GetMontageData(animIndex).AnimMontage, (*MontageData)->GetMontageData(animIndex).PlayRatio, (*MontageData)->GetMontageData(animIndex).StartSection);
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


/** F B L R -> 0 1 2 3 -> 0 2 1 3 �ִϸ��̼� �ݴ�� */
int32 ACCombatantCharacter::GetHitDirection(const FVector& start, const FVector& end , float& outAngle, bool& outbIsLeft)
{
	// #1. Ÿ������ ���ϴ� ���� ���ϱ�
	FVector toTarget = end - start;
	FVector forward = CSub::GetCustomForwardVector(this);
	toTarget.Z = 0.f;
	toTarget.Normalize();

	// #2. ������ ������ ��ȯ
	float radian = toTarget | forward;
	float angle = acos(radian);
	outAngle = angle * (180 / PI);

	// #3. �������� ���������� ����
	FVector crossProduct = toTarget ^ forward;
	outbIsLeft = crossProduct.Z > 0 ? true : false;

	// Front
	if (outAngle <= 45.f)
	{
		return 0;
	}
	else if (outAngle <= 135.f)
	{
		// �ִϸ��̼� �ݴ��
		// Left 
		if (outbIsLeft == true)
		{
			return 2;
		}
		// Right
		else
		{
			return 3;
		}
	}
	// Back
	else
	{
		return 1;
	}
}

