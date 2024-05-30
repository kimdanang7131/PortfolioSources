#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Enums/CEBehaviorType.h"

#include "CBehaviorComponent.generated.h"

/** 이 Delegate를 통해 새로운 Enum 들어올때마다 broadCast */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBehaviorTypeChanged, EBehaviorType, InPrevType, EBehaviorType, InNewType);
DECLARE_DYNAMIC_DELEGATE(FOnStopLogic);

/** BlackBoard와 상호작용을 통해 ->  Enum , Player를 등록 / Enum , Player 키를 받음  */
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
	/** BlackBoard 전용 키 */
	UPROPERTY(EditAnywhere)
		FName BehaviorKey = "Behavior";

	UPROPERTY(EditAnywhere)
		FName PlayerKey = "Player";


	/** BlackBoard에 설정된 Player 가져오기 */
public:
	class ACPlayer* GetTargetPlayer();

	void ChangeType(EBehaviorType InType);
	EBehaviorType GetType();

private:
	class UBlackboardComponent* Blackboard;
};


//public:	
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;