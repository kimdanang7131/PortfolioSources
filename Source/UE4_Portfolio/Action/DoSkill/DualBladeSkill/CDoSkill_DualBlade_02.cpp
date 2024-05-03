#include "Action/DoSkill/DualBladeSkill/CDoSkill_DualBlade_02.h"
#include "Global.h"
#include "GameFramework/Character.h"

void ACDoSkill_DualBlade_02::BeginPlay()
{
	Super::BeginPlay();

}

void ACDoSkill_DualBlade_02::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActivated)
	{
		FRotator ownerRotator = OwnerCharacter->GetActorRotation();
		FVector forwardVector = FQuat(ownerRotator).GetForwardVector();

		float temp = speed * converter;

		FVector DeltaLocation = forwardVector * temp;
		OwnerCharacter->AddActorWorldOffset(DeltaLocation, true);
	}
}

void ACDoSkill_DualBlade_02::Activate()
{
	Super::Activate();

	converter = -1.f;
	speed = 4.f;
}

void ACDoSkill_DualBlade_02::Deactivate()
{
	Super::Deactivate();
}

void ACDoSkill_DualBlade_02::DoSubSkill()
{
	converter *= -1.f;
	speed = 14.f;
}

