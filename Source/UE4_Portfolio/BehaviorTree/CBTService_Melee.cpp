#include "BehaviorTree/CBTService_Melee.h"
#include "Global.h"
///////////////////////
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
///////////////////////
#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"
#include "CAIController.h"


UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// #1. Service 를 위해 가져와야할 정보들 
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCBehaviorComponent* BehaviorComp = Cast<UCBehaviorComponent>(Controller->GetComponentByClass(UCBehaviorComponent::StaticClass()));
	ACEnemy* Enemy = Cast<ACEnemy>(Controller->GetPawn());
	UCStateComponent* State = Cast<UCStateComponent>(Enemy->GetComponentByClass(UCStateComponent::StaticClass()));
	UCStatusComponent* Status = Cast<UCStatusComponent>(Enemy->GetComponentByClass(UCStatusComponent::StaticClass()));



	if (State->IsDeadMode())
	{
		if(!BehaviorComp->IsDeadMode())
			BehaviorComp->SetDeadMode();
		return;
	}

	// #2. 히트 당하면 AIController->BehaviorComp로 Enum 변했다고 알려주기
	if (State->IsHittedMode())
	{
		BehaviorComp->SetHittedMode();
		Status->SetMovementSpeed(0);
		return;
	}

	// #3. AIController->BehaviorComp에서 플레이어 가져와서 없다면 Patrol모드
	ACPlayer* Target = BehaviorComp->GetTargetPlayer();
	if (Target == NULL)
	{
		// 패트롤모드
		BehaviorComp->SetPatrolMode();
		return;
	}
	// #4. 타겟이 있을 때 
	else
	{
		UCStateComponent* TargetState = Cast<UCStateComponent>(Target->GetComponentByClass(UCStateComponent::StaticClass()));

		// #5.Target이 죽었으면 Idle모드로 전환
		if (TargetState->IsDeadMode())
		{
			BehaviorComp->SetIdleMode();
			return;
		}
		
		// #6. distance가 AIContrller에서 설정한 MeleeActionRange안에 있다면 공격
		float distance = Enemy->GetDistanceTo(Target);
		if (distance < Controller->GetMeleeActionRange())
		{
			BehaviorComp->SetActionMode();
			return;
		}

		// #7. 공격 범위가 아니고 시야 안에 있는 상태라면 추적
		if (distance < Controller->GetSightRadius())
		{
			BehaviorComp->SetApproachMode();
			return;
		}
	}
}
