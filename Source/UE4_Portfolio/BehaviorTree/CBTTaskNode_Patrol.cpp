#include "BehaviorTree/CBTTaskNode_Patrol.h"
#include "Global.h"

#include "CAIController.h"
#include "Characters/CEnemy.h"

#include "Components/CBehaviorComponent.h"
#include "Components/CPatrolRouteComponent.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";
}

/// <summary>
/// Owner�� ������ �ִ� PatrolComp���� ������ �ִ� PatrolRoute�� ����Ǿ��ٸ�
/// ������ ��Ʈ�� ���� �̵��ϴ� ����
/// </summary>
/// <param name="OwnerComp"></param>
/// <param name="NodeMemory"></param>
/// <EBTNodeResult::Type></returns>
EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// #1. PatrolRouteComp�� �ѹ��� ��Ͻ�Ű�� ���ؼ�
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy* Enemy = Cast<ACEnemy>(Controller->GetPawn());

	if (PatrolRouteComp == nullptr)
	{
		PatrolRouteComp = Cast<UCPatrolRouteComponent>(Enemy->GetComponentByClass(UCPatrolRouteComponent::StaticClass()));
	}

	// #2. PatrolComponent�� �������� ������ Route�� �ִٸ�
	if (PatrolRouteComp)
	{
		FVector location = FVector::ZeroVector;

		// #3. ������ location & ���۷����� ��Ƽ� �����ͼ� ����
		if (PatrolRouteComp->GetDestination(location))
		{
			EPathFollowingRequestResult::Type Result;
			Result = Controller->MoveToLocation(location, AcceptanceRadius);

			switch (Result)
			{
			// �� , ��ֹ� ���� ���� �����ִ� ������ Fail
			case EPathFollowingRequestResult::Failed:
				return EBTNodeResult::Failed;

			// �������� �����ߴٸ� ���� ��Ʈ�� �̵�
			case EPathFollowingRequestResult::AlreadyAtGoal:
				PatrolRouteComp->SetIndexToNext();
				return EBTNodeResult::Succeeded;

			// ���������� �����ߴٸ� Succedded ( ���� Selector�� ������ �ݺ� )
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
