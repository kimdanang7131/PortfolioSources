#include "Components/CBehaviorComponent.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"


UCBehaviorComponent::UCBehaviorComponent()
{

}


void UCBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

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


/////////////////////////////////////////////////////////



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


void UCBehaviorComponent::ChangeType(EBehaviorType InType)
{
	EBehaviorType type = GetType();
	Blackboard->SetValueAsEnum(BehaviorKey, (uint8)InType);

	if (OnBehaviorTypeChanged.IsBound())
	{
		OnBehaviorTypeChanged.Broadcast(type, InType);
	}
}



/////////////////////////////////////////////////////////


ACPlayer* UCBehaviorComponent::GetTargetPlayer()
{
	return Cast<ACPlayer>(Blackboard->GetValueAsObject(PlayerKey));
}

EBehaviorType UCBehaviorComponent::GetType()
{
	return (EBehaviorType)Blackboard->GetValueAsEnum(BehaviorKey);
}



