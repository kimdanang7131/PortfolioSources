#include "CAIController.h"
#include "Global.h"

#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"
//#include "Components/CBehaviorComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"



ACAIController::ACAIController()
{
	
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	OwnerEnemy = Cast<ACEnemy>(InPawn);

}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

}