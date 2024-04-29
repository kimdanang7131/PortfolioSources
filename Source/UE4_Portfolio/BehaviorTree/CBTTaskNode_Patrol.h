#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Patrol.generated.h"

UCLASS()
class UE4_PORTFOLIO_API UCBTTaskNode_Patrol : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_Patrol();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


public:
	// 도착했는지 여부 파악 크기
	UPROPERTY(EditAnywhere)
		float AcceptanceRadius = 50.f;

private:
	class UCPatrolRouteComponent* PatrolRouteComp;
};
