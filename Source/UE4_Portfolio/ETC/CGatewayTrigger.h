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

	/** �ε����� NPC �̸� �߰� �ϰų� ���� ��Ʈ���� ������ ���� ��� ���� */
	UPROPERTY(EditAnywhere, Category = "Trigger")
		class UBoxComponent* Trigger;

public:
	UFUNCTION()
		virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
