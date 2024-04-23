#include "Components/CTargetComponent.h"
#include "Global.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

