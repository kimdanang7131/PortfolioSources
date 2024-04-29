#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "CPatrolRoute.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACPatrolRoute : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPatrolRoute();

public:
	FORCEINLINE USplineComponent* GetSpline() { return Spline; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spline")
		class USplineComponent* Spline;
};
