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

	/** AIController�� Enemy�� ���� �����Ҷ� ���� */
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	/** MeleeActionRange�� ���� Trace�ϴٰ� �����ȿ� �� ���� Enum -> Action���� �ٲ� */
public:
	FORCEINLINE float GetMeleeActionRange() { return MeleeActionRange; }

	/** Perception ��ü�� �ִ� �ٲ�� �˷��ִ� Delegate ��� */
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	/** �׾��� �� BrainComponent�� StopLogic�� ���� */
	UFUNCTION(BlueprintCallable)
		void ExecuteStopLogic();

private:
	/** �⺻������ ����Ǿ��ִ� Component */
	UPROPERTY(EditDefaultsOnly)
		class UAIPerceptionComponent* Perception;

	/** ���� ���� Component */
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
