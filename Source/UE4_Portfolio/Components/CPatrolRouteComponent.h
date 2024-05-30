#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPatrolRouteComponent.generated.h"


/** Level���� BP PatrolRoute����� ��� -> Spline�� �̿��Ͽ� ��ȣ�ۿ� */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCPatrolRouteComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPatrolRouteComponent();

protected:
	virtual void BeginPlay() override;

public:
	/** ���� Spline Index�� ��ġ �������� */
	UFUNCTION(BlueprintCallable, Category = "PatrolRoute")
		bool GetDestination(FVector& OutLocation);

	/** Closed Loop���� Ȯ���ؼ� ���� Index �������� */
	UFUNCTION(BlueprintCallable, Category = "PatrolRoute")
		void SetIndexToNext();
	
public:
	/** �������� ����� BP PatrolRoute */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spline")
		class ACPatrolRoute* PatrolRoute;

private:
	/** �������� ����� BP PatrolRoute���� Spline �������� */
	class USplineComponent* Spline;

	int32 routeIndex = 0;
	int32 routeMax = 0;
	int converter  = 1;
};
