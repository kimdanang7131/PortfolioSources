#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/CEStateType.h"
#include "CStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsDodgeMode() { return Type == EStateType::Dodge; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsSkillMode() { return Type == EStateType::Skill; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }


	FORCEINLINE const EStateType& GetStateNow() { return Type; }
public:
	void SetIdleMode();
	void SetDodgeMode();
	void SetActionMode();
	void SetSkillMode();
	void SetHittedMode();
	void SetDeadMode();

private:
	void ChangeType(EStateType InType);

public:
	UPROPERTY(BlueprintAssignable)
		FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type;
};
