#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enums/CEBehaviorType.h"
#include "CAIController.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACAIController : public AAIController
{
	GENERATED_BODY()

private:
	ACAIController();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

	/** AIController가 Enemy에 게임 시작할때 빙의 */
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	/** MeleeActionRange를 통해 Trace하다가 범위안에 든 순간 Enum -> Action으로 바뀜 */
public:
	FORCEINLINE float GetMeleeActionRange() { return MeleeActionRange; }

	/** Perception 자체에 있는 바뀌면 알려주는 Delegate 등록 */
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	/** 죽었을 때 BrainComponent의 StopLogic을 실행 */
	UFUNCTION(BlueprintCallable)
		void ExecuteStopLogic();

private:
	/** 기본적으로 내장되어있는 Component */
	UPROPERTY(EditDefaultsOnly)
		class UAIPerceptionComponent* Perception;

	/** 내가 만든 Component */
	UPROPERTY(EditDefaultsOnly)
		class UCBehaviorComponent* BehaviorComp;

private:
	UPROPERTY(EditAnywhere)
		float MeleeActionRange = 150.f;

	UPROPERTY(EditAnywhere)
		bool bDrawDebug = true;

	UPROPERTY(EditAnywhere)
		float AdjustCircleHeight = 50;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;
public:
	float GetSightRadius();
	
	UFUNCTION()
		void OnBehaviorChanged(EBehaviorType InPrevType, EBehaviorType InNewType);

private:
	class UAISenseConfig_Sight* Sight;
	class ACEnemy* OwnerEnemy;
};	
