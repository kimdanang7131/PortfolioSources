#include "Action/DoSkill/DualBladeSkill/CDoSkill_DualBlade_02.h"
#include "Global.h"
////////////////////
#include "GameFramework/Character.h"

void ACDoSkill_DualBlade_02::BeginPlay()
{
	Super::BeginPlay();
}

void ACDoSkill_DualBlade_02::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// #1. 스킬이 활성화 되면 일정시간동안 뒤로 가다가 Notify를 통해 
	//     Converter가 반대로 바뀌어 앞으로 돌진
	if (bIsActivated)
	{
		FRotator ownerRotator = OwnerCharacter->GetActorRotation();
		FVector forward = FQuat(ownerRotator).GetForwardVector();

		float newOffset = speed * converter;

		FVector DeltaLocation = forward * newOffset;
		OwnerCharacter->AddActorWorldOffset(DeltaLocation, true);
	}
}

/** 스킬 활성화 되면 속도 , 방향 설정 */
void ACDoSkill_DualBlade_02::Activate()
{
	Super::Activate();

	converter = -1.f;
	speed     = 6.f;
}

void ACDoSkill_DualBlade_02::Deactivate()
{
	Super::Deactivate();
}

/** Notify를 통하여 속도 , 방향 바꾸기 */
void ACDoSkill_DualBlade_02::DoSubSkill()
{
	converter *= -1.f;
	speed      = 18.f;
}

