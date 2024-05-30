#include "BehaviorTree/CBTTaskNode_Patrol.h"
#include "Global.h"
////////////////////////
#include "Components/CBehaviorComponent.h"
#include "Components/CPatrolRouteComponent.h"
////////////////////////
#include "Characters/CEnemy.h"
#include "CAIController.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";
}

/** Owner가 가지고 있는 PatrolComp에서 레벨에 있는 PatrolRoute가 적용되었다면 지정한 루트에 따라 이동하는 로직 */

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// #1. PatrolRouteComp를 한번만 등록시키기 위해서
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());

	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	ACEnemy* Enemy = Cast<ACEnemy>(Controller->GetPawn());
	if (!Enemy)
	{
		return EBTNodeResult::Failed;
	}
	// 만먁에 NPC넣을거면 따로 만들어줘야함
	// 함수 만들어서 EBTNodeResult::Type GetTaskResult(); 해서 


	UCPatrolRouteComponent* PatrolRouteComp = Cast<UCPatrolRouteComponent>(Enemy->GetComponentByClass(UCPatrolRouteComponent::StaticClass()));

	// #2. PatrolComponent에 레벨에서 적용한 Route가 있다면
	if (PatrolRouteComp)
	{
		FVector location = FVector::ZeroVector;
		// #3. 목적지 location & 레퍼런스로 담아서 가져와서 진행
		if (PatrolRouteComp->GetDestination(location))
		{
		
			EPathFollowingRequestResult::Type Result;
			Result = Controller->MoveToLocation(location, AcceptanceRadius);
			

			switch (Result)
			{
			// 벽 , 장애물 등을 통해 막혀있는 곳에서 Fail
			case EPathFollowingRequestResult::Failed:
				return EBTNodeResult::Failed;

			// 목적지에 도착했다면 다음 루트로 이동
			case EPathFollowingRequestResult::AlreadyAtGoal:
				PatrolRouteComp->SetIndexToNext();
				return EBTNodeResult::Succeeded;

			// 성공적으로 도착했다면 Succedded ( 현재 Selector라서 어차피 반복 )
			case EPathFollowingRequestResult::RequestSuccessful:
				return EBTNodeResult::Succeeded;

			default:
				break;
			}
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::InProgress;
}
