// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPatrolRouteComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCPatrolRouteComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPatrolRouteComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable, Category = "PatrolRoute")
		bool GetDestination(FVector& OutLocation);

	UFUNCTION(BlueprintCallable, Category = "PatrolRoute")
		void SetIndexToNext();
	
public:
	// 레벨에서 등록할 BP PatrolRoute 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spline")
		class ACPatrolRoute* PatrolRoute;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class ACharacter* OwnerCharacter;

private:
	class USplineComponent* Spline;

	int32 routeIndex = 0;
	int32 routeMax = 0;
	int converter  = 1;
};
