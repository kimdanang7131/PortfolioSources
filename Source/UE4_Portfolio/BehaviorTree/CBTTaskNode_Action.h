#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Action.generated.h"

UCLASS()
class UE4_PORTFOLIO_API UCBTTaskNode_Action : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_Action();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "AI")
		float Delay = 2.0f;

private:
	float TotalTime = 0.0f;

};
