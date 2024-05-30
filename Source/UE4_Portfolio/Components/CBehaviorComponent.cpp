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

/** BTS_Melee에서 State검사후 거기에 맞춰서 BehaviorState 변경 -> 여기서 BlackBoard Enum 변경 */
void UCBehaviorComponent::ChangeType(EBehaviorType InType)
{
	// #1. BroadCast하기 위해서 현재 Enum 받기
	EBehaviorType type = GetType();
	// #2. 새로운 변경될 Enum BlackBoard에 적용
	Blackboard->SetValueAsEnum(BehaviorKey, (uint8)InType);


	// #3. 연결된 델리게이트에 이전의 Enum을 새로운 Enum으로 바뀌었다고 전달
	if (OnBehaviorTypeChanged.IsBound())
	{
		OnBehaviorTypeChanged.Broadcast(type, InType);
	}
}

/////////////////////////////////////////////////////////

/** BlackBoard에 설정된 Player 가져오기 */
ACPlayer* UCBehaviorComponent::GetTargetPlayer()
{
	return Cast<ACPlayer>(Blackboard->GetValueAsObject(PlayerKey));
}

/** BlackBoard에 BehaivorEnum 키 가져오기 */
EBehaviorType UCBehaviorComponent::GetType()
{
	return (EBehaviorType)Blackboard->GetValueAsEnum(BehaviorKey);
}



