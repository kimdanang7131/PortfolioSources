#include "Components/CStateComponent.h"

UCStateComponent::UCStateComponent()
{

}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

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

// 델리게이트 BroadCast 
// Player에서 이 델리게이트를 구독하고 State->Set~~Mode를 하면 자동으로
// 이 함수로 오게되고 BroadCast를 통하여 각각의 Character의 타입 바꾸기
// 기본적으로 DYNAMIC_TWOPARAMS로 되어있고 이전타입 , 현재타입 넘겨서 같으면 아무일도 일어나지 않도록
void UCStateComponent::ChangeType(EStateType InType)
{
	EStateType prevType = Type;
	Type = InType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, InType);
}




