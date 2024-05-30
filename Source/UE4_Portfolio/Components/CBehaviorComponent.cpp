#include "Components/CBehaviorComponent.h"
#include "Global.h"
////////////////////
#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"
#include "Components/CStatusComponent.h"
////////////////////
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


UCBehaviorComponent::UCBehaviorComponent()
{

}

void UCBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIdleMode();
}

#pragma region IsMode()
bool UCBehaviorComponent::IsIdleMode()
{
	return GetType() == EBehaviorType::Idle_BT;
}

bool UCBehaviorComponent::IsDodgeMode()
{
	return GetType() == EBehaviorType::Dodge_BT;
}

bool UCBehaviorComponent::IsActionMode()
{
	return GetType() == EBehaviorType::Action_BT;
}

bool UCBehaviorComponent::IsSkillMode()
{
	return GetType() == EBehaviorType::Skill_BT;
}

bool UCBehaviorComponent::IsHittedMode()
{
	return GetType() == EBehaviorType::Hitted_BT;
}

bool UCBehaviorComponent::IsApproachMode()
{
	return GetType() == EBehaviorType::Approach_BT;
}

bool UCBehaviorComponent::IsPatrolMode()
{
	return GetType() == EBehaviorType::Patrol_BT;
}

bool UCBehaviorComponent::IsDeadMode()
{
	return GetType() == EBehaviorType::Dead_BT;
}
#pragma endregion

/////////////////////////////////////////////////////////

#pragma region SetMode()
void UCBehaviorComponent::SetIdleMode()
{
	ChangeType(EBehaviorType::Idle_BT);
}

void UCBehaviorComponent::SetDodgeMode()
{
	ChangeType(EBehaviorType::Dodge_BT);
}

void UCBehaviorComponent::SetActionMode()
{
	ChangeType(EBehaviorType::Action_BT);
}

void UCBehaviorComponent::SetSkillMode()
{
	ChangeType(EBehaviorType::Skill_BT);
}

void UCBehaviorComponent::SetHittedMode()
{
	ChangeType(EBehaviorType::Hitted_BT);
}

void UCBehaviorComponent::SetApproachMode()
{
	ChangeType(EBehaviorType::Approach_BT);
}

void UCBehaviorComponent::SetPatrolMode()
{
	ChangeType(EBehaviorType::Patrol_BT);
}

void UCBehaviorComponent::SetDeadMode()
{
	ChangeType(EBehaviorType::Dead_BT);
	//OnStopLogic.ExecuteIfBound();
}
#pragma endregion

/** BTS_Melee���� State�˻��� �ű⿡ ���缭 BehaviorState ���� -> ���⼭ BlackBoard Enum ���� */
void UCBehaviorComponent::ChangeType(EBehaviorType InType)
{
	// #1. BroadCast�ϱ� ���ؼ� ���� Enum �ޱ�
	EBehaviorType type = GetType();
	// #2. ���ο� ����� Enum BlackBoard�� ����
	Blackboard->SetValueAsEnum(BehaviorKey, (uint8)InType);


	// #3. ����� ��������Ʈ�� ������ Enum�� ���ο� Enum���� �ٲ���ٰ� ����
	if (OnBehaviorTypeChanged.IsBound())
	{
		OnBehaviorTypeChanged.Broadcast(type, InType);
	}
}

/////////////////////////////////////////////////////////

/** BlackBoard�� ������ Player �������� */
ACPlayer* UCBehaviorComponent::GetTargetPlayer()
{
	return Cast<ACPlayer>(Blackboard->GetValueAsObject(PlayerKey));
}

/** BlackBoard�� BehaivorEnum Ű �������� */
EBehaviorType UCBehaviorComponent::GetType()
{
	return (EBehaviorType)Blackboard->GetValueAsEnum(BehaviorKey);
}



