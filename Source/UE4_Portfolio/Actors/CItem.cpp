
#include "Actors/CItem.h"
#include "Global.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

ACItem::ACItem()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACItem::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AActor>(GetOwner());
	
	if (!!OwnerCharacter)
	{
		//State = Cast<UCStateComponent>(GetComponentByClass(UCStateComponent::StaticClass()));
		State  = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
		Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	}
}



// Called every frame
void ACItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

