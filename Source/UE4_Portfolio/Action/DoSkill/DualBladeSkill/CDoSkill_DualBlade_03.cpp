#include "Action/DoSkill/DualBladeSkill/CDoSkill_DualBlade_03.h"
#include "Global.h"



void ACDoSkill_DualBlade_03::BeginPlay()
{
	Super::BeginPlay();
	bCanRotate = true;
	rInterpSpeed = 3.f;
}

void ACDoSkill_DualBlade_03::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACDoSkill_DualBlade_03::Activate()
{
	Super::Activate();
}

void ACDoSkill_DualBlade_03::Deactivate()
{
	Super::Deactivate();

}

