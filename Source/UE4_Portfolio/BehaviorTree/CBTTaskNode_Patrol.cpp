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

/** Owner�� ������ �ִ� PatrolComp���� ������ �ִ� PatrolRoute�� ����Ǿ��ٸ� ������ ��Ʈ�� ���� �̵��ϴ� ���� */

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// #1. PatrolRouteComp�� �ѹ��� ��Ͻ�Ű�� ���ؼ�
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
	// ���п� NPC�����Ÿ� ���� ����������
	// �Լ� ���� EBTNodeResult::Type GetTaskResult(); �ؼ� 


	UCPatrolRouteComponent* PatrolRouteComp = Cast<UCPatrolRouteComponent>(Enemy->GetComponentByClass(UCPatrolRouteComponent::StaticClass()));

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
