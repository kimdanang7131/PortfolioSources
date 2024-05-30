#include "Components/CStateComponent.h"

UCStateComponent::UCStateComponent()
{
}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

#pragma region SetMode()
void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetDodgeMode()
{
	ChangeType(EStateType::Dodge);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetSkillMode()
{
	ChangeType(EStateType::Skill);
}

void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}
#pragma endregion

/** Type�� �ٲ���� �� ����� Delegate�� ���� Character���� �ű⿡ �´� StateMontageMap �ִϸ��̼� ��� �� �ű⿡ �´� �Լ� ���� */
void UCStateComponent::ChangeType(EStateType InType)
{
	EStateType prevType = Type;
	Type = InType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, InType);
}




