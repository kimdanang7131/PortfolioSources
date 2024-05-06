#include "Action/DoSkill/DualBladeSkill/CDoSkill_DualBlade_03.h"
#include "Global.h"

/** 회전 가능하게 설정 , 회전 스피드 설정 */
void ACDoSkill_DualBlade_03::BeginPlay()
{
	Super::BeginPlay();
	bCanRotate = true;
	rInterpSpeed = 5.f;
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

