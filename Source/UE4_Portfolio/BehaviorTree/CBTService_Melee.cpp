#include "BehaviorTree/CBTService_Melee.h"

#include "Global.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"
#include "CAIController.h"

#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"


UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCBehaviorComponent* BehaviorComp = Cast<UCBehaviorComponent>(Controller->GetComponentByClass(UCBehaviorComponent::StaticClass()));

	ACEnemy* Enemy = Cast<ACEnemy>(Controller->GetPawn());
	UCStateComponent* State = Cast<UCStateComponent>(Enemy->GetComponentByClass(UCStateComponent::StaticClass()));
	UCStatusComponent* Status = Cast<UCStatusComponent>(Enemy->GetComponentByClass(UCStatusComponent::StaticClass()));

	// #1. 히트 당하면 AIController->BehaviorComp로 Enum 변했다고 알려주기
	if (State->IsHittedMode())
	{
		BehaviorComp->SetHittedMode();
		Status->SetMovementSpeed(0);
		return;
	}

	// #2. AIController->BehaviorComp에서 플레이어 가져와서 없다면 Idle , 있다면 죽었는지 체크
	ACPlayer* Target = BehaviorComp->GetTargetPlayer();
	if (Target == NULL)
	{
		// 패트롤모드
		BehaviorComp->SetPatrolMode();
		return;
	}
	else
	{
		UCStateComponent* TargetState = Cast<UCStateComponent>(Target->GetComponentByClass(UCStateComponent::StaticClass()));

		if (TargetState->IsDeadMode())
		{
			BehaviorComp->SetIdleMode();
			return;
		}

		// #3. 플레이어가 있고 죽지도 않은 상태라면 범위안에 있는지 체크하고 Attack
		float distance = Enemy->GetDistanceTo(Target);

		if (distance < Controller->GetMeleeActionRange())
		{
			BehaviorComp->SetActionMode();
			return;
		}

		// #4. 공격 범위가 아니고 시야 안에 있는 상태라면 추적
		if (distance < Controller->GetSightRadius())
		{
			BehaviorComp->SetApproachMode();
			return;
		}
	}
}
