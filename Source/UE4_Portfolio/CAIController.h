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

public:
	FORCEINLINE float GetMeleeActionRange() { return MeleeActionRange; }
	
protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	UPROPERTY(EditDefaultsOnly)
		class UAIPerceptionComponent* Perception;

	UPROPERTY(EditDefaultsOnly)
		class UCBehaviorComponent* BehaviorComp;

public:
	float GetSightRadius();

private:
	UPROPERTY(EditAnywhere)
		float MeleeActionRange = 150.f;

	UPROPERTY(EditAnywhere)
		bool bDrawDebug = true;

	UPROPERTY(EditAnywhere)
		float AdjustCircleHeight = 50;



	//UPROPERTY(EditAnywhere, Category = "AISenseConfig")
		

private:
	class ACEnemy* OwnerEnemy;
	class UAISenseConfig_Sight* Sight;
};	
