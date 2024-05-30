#include "BehaviorTree/CBTTaskNode_Action.h"
#include "Global.h"
///////////////////////
#include "Components/CWeaponStateComponent.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"
///////////////////////
#include "CAIController.h"
#include "Characters/CEnemy.h"
#include "Characters/CPlayer.h"


UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	bNotifyTick = true;
	NodeName = "Action";
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	// #1. Task�� ���� �����;��� ������
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy* Enemy = Cast<ACEnemy>(Controller->GetPawn());
	UCStateComponent* EnemyState = CHelpers::GetComponent<UCStateComponent>(Enemy);
	UCBehaviorComponent* EnemyBehaviorComp = Cast<UCBehaviorComponent>(Controller->GetComponentByClass(UCBehaviorComponent::StaticClass()));
	UCWeaponStateComponent* WeaponState    = Cast<UCWeaponStateComponent>(Enemy->GetComponentByClass(UCWeaponStateComponent::StaticClass()));

	// �÷��̾� ����
	ACPlayer* Target = EnemyBehaviorComp->GetTargetPlayer();

	// #2. Target(Player)������ �Ÿ� ���ؼ� AIController�κ��� ���� MeleeActionRange �ȿ� �ִٸ� ���� , ���������� Target(Player) �������� ȸ��
	if (Target != nullptr && !EnemyBehaviorComp->IsDeadMode())
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

	// #3. ActionRange�� ����� Idle�� 
	if (EnemyState->IsIdleMode())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}












//	//ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	//ACEnemy* Enemy = Cast<ACEnemy>(Controller->GetPawn());
	//
	//UCWeaponStateComponent* WeaponState = Cast<UCWeaponStateComponent>(Enemy->GetComponentByClass(UCWeaponStateComponent::StaticClass()));
	//UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(Enemy);

	//UCBehaviorComponent* BehaviorComp = Cast<UCBehaviorComponent>(Controller->GetComponentByClass(UCBehaviorComponent::StaticClass()));
	//ACPlayer* Target = BehaviorComp->GetTargetPlayer();

	//WeaponState->DoAction();