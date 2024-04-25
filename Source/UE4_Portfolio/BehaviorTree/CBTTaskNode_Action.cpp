#include "BehaviorTree/CBTTaskNode_Action.h"

#include "Global.h"
#include "CAIController.h"
#include "Characters/CEnemy.h"

#include "Components/CWeaponStateComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	bNotifyTick = true;

	NodeName = "Action";
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy* Enemy = Cast<ACEnemy>(Controller->GetPawn());
	UCWeaponStateComponent* WeaponState = Cast<UCWeaponStateComponent>(Controller->GetComponentByClass(UCWeaponStateComponent::StaticClass()));

	TotalTime = 0.0f;
	WeaponState->DoAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy* Enemy = Cast<ACEnemy>(Controller->GetPawn());
	UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(Enemy);

	TotalTime += DeltaSeconds;

	if (State->IsIdleMode() && TotalTime > Delay)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}