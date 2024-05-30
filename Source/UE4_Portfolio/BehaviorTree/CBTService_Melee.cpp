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

	// #1. Service �� ���� �����;��� ������ 
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

	// #2. ��Ʈ ���ϸ� AIController->BehaviorComp�� Enum ���ߴٰ� �˷��ֱ�
	if (State->IsHittedMode())
	{
		BehaviorComp->SetHittedMode();
		Status->SetMovementSpeed(0);
		return;
	}

	// #3. AIController->BehaviorComp���� �÷��̾� �����ͼ� ���ٸ� Patrol���
	ACPlayer* Target = BehaviorComp->GetTargetPlayer();
	if (Target == NULL)
	{
		// ��Ʈ�Ѹ��
		BehaviorComp->SetPatrolMode();
		return;
	}
	// #4. Ÿ���� ���� �� 
	else
	{
		UCStateComponent* TargetState = Cast<UCStateComponent>(Target->GetComponentByClass(UCStateComponent::StaticClass()));

		// #5.Target�� �׾����� Idle���� ��ȯ
		if (TargetState->IsDeadMode())
		{
			BehaviorComp->SetIdleMode();
			return;
		}
		
		// #6. distance�� AIContrller���� ������ MeleeActionRange�ȿ� �ִٸ� ����
		float distance = Enemy->GetDistanceTo(Target);
		if (distance < Controller->GetMeleeActionRange())
		{
			BehaviorComp->SetActionMode();
			return;
		}

		// #7. ���� ������ �ƴϰ� �þ� �ȿ� �ִ� ���¶�� ����
		if (distance < Controller->GetSightRadius())
		{
			BehaviorComp->SetApproachMode();
			return;
		}
	}
}
