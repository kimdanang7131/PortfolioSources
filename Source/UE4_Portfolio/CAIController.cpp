#include "CAIController.h"
#include "Global.h"

#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"

#include "Components/CBehaviorComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");
	BehaviorComp = CreateDefaultSubobject<UCBehaviorComponent>("Behavior");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");

	Sight->SightRadius = 900;
	Sight->LoseSightRadius = 1200;
	Sight->PeripheralVisionAngleDegrees = 90;
	Sight->SetMaxAge(2);

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerEnemy = Cast<ACEnemy>(InPawn); 
	SetGenericTeamId(OwnerEnemy->GetTeamID());

	// Perception�� �ٲ�� �˷���
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);

	// ���⼭ ����� Blackboard���ٰ� �÷��̾�� ����� Blackboard�����ͼ� ��� , Component���� ��Ͻ�����
	UseBlackboard(OwnerEnemy->GetBehaviorTree()->GetBlackboardAsset(), Blackboard);
	BehaviorComp->SetBlackboard(Blackboard);

	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	Perception->OnPerceptionUpdated.Clear();
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FALSE_RETURN(bDrawDebug);

	FVector center = OwnerEnemy->GetActorLocation();
	center.Z -= AdjustCircleHeight;
	DrawDebugCircle(GetWorld(), center, Sight->SightRadius, 300, FColor::Green, false, -1, 0, 0, FVector::RightVector, FVector::ForwardVector);
	DrawDebugCircle(GetWorld(), center, MeleeActionRange, 300, FColor::Red, false, -1, 0, 0, FVector::RightVector, FVector::ForwardVector);
}

float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}

// #1. Perception�� �ִ� �⺻ ��������Ʈ OneParam
void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), actors);

	// #2. ������ Actor�� �߿��� Player�� ��������
	ACPlayer* Player = NULL;
	for (AActor* actor : actors)
	{
		Player = Cast<ACPlayer>(actor);

		if (!!Player)
			break;
	}

	Blackboard->SetValueAsObject("Player", Player);
}