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

// ��������Ʈ BroadCast 
// Player���� �� ��������Ʈ�� �����ϰ� State->Set~~Mode�� �ϸ� �ڵ�����
// �� �Լ��� ���Եǰ� BroadCast�� ���Ͽ� ������ Character�� Ÿ�� �ٲٱ�
// �⺻������ DYNAMIC_TWOPARAMS�� �Ǿ��ְ� ����Ÿ�� , ����Ÿ�� �Ѱܼ� ������ �ƹ��ϵ� �Ͼ�� �ʵ���
void UCStateComponent::ChangeType(EStateType InType)
{
	EStateType prevType = Type;
	Type = InType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, InType);
}




