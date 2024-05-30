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

/** 피격 당했을 때 */
float ACCombatantCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageInstigator = EventInstigator;
	DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	// #1. 공격자의 LaunchAmount를 가져와서 그 양만큼 날아가게 세팅해줘야함 ( 맞은 놈 기준 실행되어서 )
	ACCombatantCharacter* CombatantCharacter = Cast<ACCombatantCharacter>(EventInstigator->GetPawn());
	if (CombatantCharacter)
		SetLaunchAmount(CombatantCharacter->GetLaunchAmount());

	// #2. DataAsset에 설정된 Hitted가 실행되도록 하기
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

/** 피격 당했을 때 */
void ACCombatantCharacter::Hitted()
{
	Super::Hitted();

	FVector start = GetActorLocation();
	FVector target = DamageInstigator->GetPawn()->GetActorLocation();

	hitAngle = 0.f;
	bool bIsLeft = false;
	int32 animIndex = GetHitDirection(start, target, hitAngle, bIsLeft);

	// #1. StatusComponent에 데미지 값 전달하고 0보다 작으면 DeadMode 바꾸고 return
	Status->SubHealth(DamageValue);
	DamageValue = 0.0f;

	
	if (State->IsDeadMode())
	{
		// 죽었을 때의 각도에 따라 바라보는 방향 설정
		if (hitAngle <= 90.f)
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
		else
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(target, start));

		return;
	}

	//#2. 현재 DataAsset에 등록된 애니메이션 가져오기
	UCMontageDataAsset** MontageData = StateMontageMap.Find(State->GetStateNow());
	FALSE_RETURN(*MontageData);

	//#3. SetStop을 통해 Key Input을 Ignore 시킴
	Status->SetStop();
	PlayAnimMontage((*MontageData)->GetMontageData(animIndex).AnimMontage, (*MontageData)->GetMontageData(animIndex).PlayRatio, (*MontageData)->GetMontageData(animIndex).StartSection);
	//SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
	DamageInstigator = NULL;
	
	// #4. 때린놈으로부터 맞은 놈으로 Launch 시킴
	FVector direction = start - target;
	direction.Normalize();
	direction.Z = 0.f;
	LaunchCharacter(direction * launchAmount, true, false);

	// #5. 예외상황) 때리려고하다가 맞았을 때 Action Setting 초기화로
	if (WeaponState)
	{
		if (WeaponState->GetCurrentWeapon())
			WeaponState->GetCurrentWeapon()->ResetAction();
	}
}

void ACCombatantCharacter::Dead()
{
	// #1. 앞에서 맞았는지 뒤에서 맞았는지
	int32 animIndex = hitAngle <= 90 ? 0 : 1;

	// #2. 더이상 공격받지 않게 -> 사실 SetCanDamaged가 맞긴함
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// #3. State DataAsset에 등록된 애니메이션 실행
	UCMontageDataAsset** MontageData = StateMontageMap.Find(State->GetStateNow());
	FALSE_RETURN(*MontageData);
	PlayAnimMontage((*MontageData)->GetMontageData(animIndex).AnimMontage, (*MontageData)->GetMontageData(animIndex).PlayRatio, (*MontageData)->GetMontageData(animIndex).StartSection);
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


/** F B L R -> 0 1 2 3 -> 0 2 1 3 애니메이션 반대로 */
int32 ACCombatantCharacter::GetHitDirection(const FVector& start, const FVector& end , float& outAngle, bool& outbIsLeft)
{
	// #1. 타겟으로 향하는 벡터 구하기
	FVector toTarget = end - start;
	FVector forward = CSub::GetCustomForwardVector(this);
	toTarget.Z = 0.f;
	toTarget.Normalize();

	// #2. 라디안을 각도로 변환
	float radian = toTarget | forward;
	float angle = acos(radian);
	outAngle = angle * (180 / PI);

	// #3. 왼쪽인지 오른쪽인지 구분
	FVector crossProduct = toTarget ^ forward;
	outbIsLeft = crossProduct.Z > 0 ? true : false;

	// Front
	if (outAngle <= 45.f)
	{
		return 0;
	}
	else if (outAngle <= 135.f)
	{
		// 애니메이션 반대로
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

