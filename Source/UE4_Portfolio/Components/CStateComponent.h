#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/CEStateType.h"
#include "CStateComponent.generated.h"

/** Type�� �ٲ���� �� ����� Delegate�� ���� Character���� �ű⿡ �´� StateMontageMap �ִϸ��̼� ��� �� �ű⿡ �´� �Լ� ���� */
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
	FORCEINLINE const EStateType& GetStateNow() { return Type; }

	UPROPERTY(BlueprintAssignable)
		FStateTypeChanged OnStateTypeChanged;

#pragma region IsMode()
public:
	UFUNCTION(BlueprintPure)
		 bool IsIdleMode() { return Type == EStateType::Idle; }

	UFUNCTION(BlueprintPure)
		 bool IsDodgeMode() { return Type == EStateType::Dodge; }

	UFUNCTION(BlueprintPure)
		 bool IsActionMode() { return Type == EStateType::Action; }

	UFUNCTION(BlueprintPure)
		 bool IsSkillMode() { return Type == EStateType::Skill; }

	UFUNCTION(BlueprintPure)
		 bool IsHittedMode() { return Type == EStateType::Hitted; }

	UFUNCTION(BlueprintPure)
		 bool IsDeadMode() { return Type == EStateType::Dead; }
#pragma endregion
	
#pragma region SetMode()
public:
	void SetIdleMode();
	void SetDodgeMode();
	void SetActionMode();
	void SetSkillMode();
	void SetHittedMode();
	void SetDeadMode();
#pragma endregion

private:
	void ChangeType(EStateType InType);

private:
	EStateType Type;
};
