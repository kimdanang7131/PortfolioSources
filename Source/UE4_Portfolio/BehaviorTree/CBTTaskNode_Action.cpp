#include "BehaviorTree/CBTTaskNode_Action.h"

#include "Global.h"
#include "CAIController.h"
#include "Characters/CEnemy.h"
#include "Characters/CPlayer.h"

#include "Components/CWeaponStateComponent.h"
#include "Components/CStateComponent.h"

#include "Components/CBehaviorComponent.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	bNotifyTick = true;
	NodeName = "Action";
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Super::ExecuteTask(OwnerComp, NodeMemory);

	//ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	//ACEnemy* Enemy = Cast<ACEnemy>(Controller->GetPawn());
	//
	//UCWeaponStateComponent* WeaponState = Cast<UCWeaponStateComponent>(Enemy->GetComponentByClass(UCWeaponStateComponent::StaticClass()));
	//UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(Enemy);

	//UCBehaviorComponent* BehaviorComp = Cast<UCBehaviorComponent>(Controller->GetComponentByClass(UCBehaviorComponent::StaticClass()));
	//ACPlayer* Target = BehaviorComp->GetTargetPlayer();


	//WeaponState->DoAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy* Enemy = Cast<ACEnemy>(Controller->GetPawn());
	UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(Enemy);
	UCBehaviorComponent* BehaviorComp = Cast<UCBehaviorComponent>(Controller->GetComponentByClass(UCBehaviorComponent::StaticClass()));

	ACPlayer* Target = BehaviorComp->GetTargetPlayer();
	UCWeaponStateComponent* WeaponState = Cast<UCWeaponStateComponent>(Enemy->GetComponentByClass(UCWeaponStateComponent::StaticClass()));
	
	if (Target != nullptr)
	{
		float distance = Enemy->GetDistanceTo(Target);
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), Target->GetActorLocation());

		FRotator NewRoatation = FMath::RInterpTo(Enemy->GetActorRotation(), TargetRotation, DeltaSeconds, rInterpSpeed);
		
		Enemy->SetActorRotation(NewRoatation);
		if (distance < Controller->GetMeleeActionRange())
		{
			WeaponState->DoAction();
		}
	}

	if (State->IsIdleMode())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}