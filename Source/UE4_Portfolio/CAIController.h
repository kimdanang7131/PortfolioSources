#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	ACAIController();

//protected:
//	virtual void BeginPlay() override;
//
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
//
//private:
//	virtual void Tick(float DeltaTime) override;
//
//	UFUNCTION()
//		void OnPerceptionUpdated(const TArray<AActor*>& UpdateActors);


private:
	//UPROPERTY(EditDefaultsOnly)
	//	class UAIPerceptionComponent* Perception;

	//UPROPERTY(EditDefaultsOnly)
	//	class UCBehaviorTree* 


private:
	class ACEnemy* OwnerEnemy;
	class UAISenseConfig_Sight* Sight;
};	
