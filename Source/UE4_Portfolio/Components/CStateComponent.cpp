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

/** Type이 바뀌었을 때 연결된 Delegate를 통해 Character에서 거기에 맞는 StateMontageMap 애니메이션 재생 및 거기에 맞는 함수 실행 */
void UCStateComponent::ChangeType(EStateType InType)
{
	EStateType prevType = Type;
	Type = InType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, InType);
}




