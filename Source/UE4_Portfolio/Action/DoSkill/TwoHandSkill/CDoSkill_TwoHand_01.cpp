#include "Action/DoSkill/TwoHandSkill/CDoSkill_TwoHand_01.h"
#include "Global.h"
///////////////////////
#include "Components/CStatusComponent.h"

void ACDoSkill_TwoHand_01::BeginPlay()
{
	Super::BeginPlay();
}

void ACDoSkill_TwoHand_01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACDoSkill_TwoHand_01::Activate()
{
	Super::Activate();
}

void ACDoSkill_TwoHand_01::Deactivate()
{
	Super::Deactivate();
}

void ACDoSkill_TwoHand_01::OnAttackByTimer()
{
	Super::OnAttackByTimer();
	Status->SetMove();
}

void ACDoSkill_TwoHand_01::OffAttackByTimer()
{
	Super::OffAttackByTimer();
	Status->SetStop();
}