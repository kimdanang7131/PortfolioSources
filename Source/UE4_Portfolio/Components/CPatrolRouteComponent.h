#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPatrolRouteComponent.generated.h"


/** Level에서 BP PatrolRoute만들고 등록 -> Spline을 이용하여 상호작용 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCPatrolRouteComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPatrolRouteComponent();

protected:
	virtual void BeginPlay() override;

public:
	/** 현재 Spline Index의 위치 가져오기 */
	UFUNCTION(BlueprintCallable, Category = "PatrolRoute")
		bool GetDestination(FVector& OutLocation);

	/** Closed Loop인지 확인해서 다음 Index 가져오기 */
	UFUNCTION(BlueprintCallable, Category = "PatrolRoute")
		void SetIndexToNext();
	
public:
	/** 레벨에서 등록할 BP PatrolRoute */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spline")
		class ACPatrolRoute* PatrolRoute;

private:
	/** 레벨에서 등록한 BP PatrolRoute안의 Spline 가져오기 */
	class USplineComponent* Spline;

	int32 routeIndex = 0;
	int32 routeMax = 0;
	int converter  = 1;
};
