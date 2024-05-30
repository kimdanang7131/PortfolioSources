#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGatewayTrigger.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACGatewayTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	ACGatewayTrigger();
protected:
	virtual void BeginPlay() override;

	/** 부딪히면 NPC 이름 뜨게 하거나 레벨 스트리밍 볼륨에 대한 기능 설정 */
	UPROPERTY(EditAnywhere, Category = "Trigger")
		class UBoxComponent* Trigger;

public:
	UFUNCTION()
		virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
