#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Enums/CEBehaviorType.h"

#include "CBehaviorComponent.generated.h"

/** �� Delegate�� ���� ���ο� Enum ���ö����� broadCast */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBehaviorTypeChanged, EBehaviorType, InPrevType, EBehaviorType, InNewType);
DECLARE_DYNAMIC_DELEGATE(FOnStopLogic);

/** BlackBoard�� ��ȣ�ۿ��� ���� ->  Enum , Player�� ��� / Enum , Player Ű�� ����  */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCBehaviorComponent();

	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }
	
protected:
	virtual void BeginPlay() override;

#pragma region IsMode()
public:
	UFUNCTION(BlueprintPure)
		bool IsIdleMode();

	UFUNCTION(BlueprintPure)
		bool IsDodgeMode();

	UFUNCTION(BlueprintPure)
		bool IsActionMode();

	UFUNCTION(BlueprintPure)
		bool IsSkillMode();

	UFUNCTION(BlueprintPure)
		bool IsHittedMode();

	UFUNCTION(BlueprintPure)
		bool IsApproachMode();

	UFUNCTION(BlueprintPure)
		bool IsPatrolMode();

	UFUNCTION(BlueprintPure)
		bool IsDeadMode();
#pragma endregion

#pragma region SetMode()
public:
	void SetIdleMode();
	void SetDodgeMode();
	void SetActionMode();
	void SetSkillMode();
	void SetHittedMode();
	void SetApproachMode();
	void SetPatrolMode();
	void SetDeadMode();
#pragma endregion

public:
	UPROPERTY(BlueprintAssignable)
		FBehaviorTypeChanged OnBehaviorTypeChanged;

	FOnStopLogic OnStopLogic;
		
private:
	/** BlackBoard ���� Ű */
	UPROPERTY(EditAnywhere)
		FName BehaviorKey = "Behavior";

	UPROPERTY(EditAnywhere)
		FName PlayerKey = "Player";


	/** BlackBoard�� ������ Player �������� */
public:
	class ACPlayer* GetTargetPlayer();

	void ChangeType(EBehaviorType InType);
	EBehaviorType GetType();

private:
	class UBlackboardComponent* Blackboard;
};


//public:	
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;