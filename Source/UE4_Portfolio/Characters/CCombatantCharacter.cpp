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

/** 피격 당했을 때 */
float ACCombatantCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageInstigator = EventInstigator;
	DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	// 공격자의 LaunchAmount를 가져와서 그 양만큼 날아가게 세팅해줘야함 ( 맞은 놈 기준 실행되어서 )
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


/** 피격 당했을 때*/
void ACCombatantCharacter::Hitted()
{
	Super::Hitted();

	// #1. StatusComponent에 데미지 값 전달하고 0보다 작으면 DeadMode
	Status->SubHealth(DamageValue);
	if (Status->GetHealth() - DamageValue <= 0.0f)
	{
		DamageValue = 0.0f;
		State->SetDeadMode();
		return;
	}

	DamageValue = 0.0f;

	// #2. 현재 DataAsset에 등록된 애니메이션 가져오기
	UCMontageDataAsset** MontageData = StateMontageMap.Find(State->GetStateNow());
	FALSE_RETURN(*MontageData);

	// #3. SetStop을 통해 Key Input을 Ignore 시킴
	Status->SetStop();
	PlayAnimMontage((*MontageData)->GetMontageData(0).AnimMontage, (*MontageData)->GetMontageData(0).PlayRatio, (*MontageData)->GetMontageData(0).StartSection);

	// #4. 맞았을 때 때린놈을 쳐다보게 설정
	FVector start = GetActorLocation();
	FVector target = DamageInstigator->GetPawn()->GetActorLocation();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
	DamageInstigator = NULL;

	// #5. 때린놈으로부터 맞은 놈으로 Launch 시킴
	FVector direction = start - target;
	direction.Normalize();
	direction.Z = 0.f;

	LaunchCharacter(direction * launchAmount, true, false);

	// #6. 예외상황) 때리려고하다가 맞았을 때 Action Setting 초기화로
	if (WeaponState)
	{
		if (WeaponState->GetCurrentWeapon())
			WeaponState->GetCurrentWeapon()->ResetAction();
	}
}

/** State DataAsset - 회피 모드 */
void ACCombatantCharacter::Dodging()
{
	FALSE_RETURN(State->IsIdleMode());
	State->SetDodgeMode();

	// #1. DataAsset이 있다면 Dodge 애니메이션 가져오기
	UCMontageDataAsset** MontageData = StateMontageMap.Find(State->GetStateNow());
	FALSE_RETURN(*MontageData);
	
	// #2. 가장 최근에 입력한 Vector 가져와서 0이면 정면 벡터로 구르기
	FVector LastVector = GetLastMovementInputVector();
	if (LastVector.IsNearlyZero())
		LastVector = CSub::GetCustomForwardVector(this);


	// #3. 마지막에 입력한 방향으로 회전 먼저 시키고 그쪽 방향으로 구르기 애니메이션 시작
	FRotator DodgeDirection = UKismetMathLibrary::MakeRotFromX(LastVector);
	SetActorRotation(DodgeDirection);

	PlayAnimMontage((*MontageData)->GetMontageData(0).AnimMontage , (*MontageData)->GetMontageData(0).PlayRatio, (*MontageData)->GetMontageData(0).StartSection);
}

