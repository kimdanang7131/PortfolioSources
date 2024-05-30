#include "ETC/CGatewayTrigger.h"
#include "Global.h"
///////////////////////
#include "Characters/CPlayer.h"
///////////////////////
#include "Components/BoxComponent.h"


ACGatewayTrigger::ACGatewayTrigger()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
}

void ACGatewayTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxOverlap);
}

void ACGatewayTrigger::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// #1. 플레이어인지 확인해서 UI켜기
	if (OtherActor->IsA<ACPlayer>())
	{
		ACPlayer* Player = Cast<ACPlayer>(OtherActor);
		if (Player)
		{
			Player->ToggleVillage();
		}
	}
}



