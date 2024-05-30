#include "CAIController.h"
#include "Global.h"
///////////////////////
#include "Components/CBehaviorComponent.h"
#include "Components/CStatusComponent.h"
///////////////////////
#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"
///////////////////////
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BrainComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	/** Blackboard�� Perception�� �⺻������ ����Ǿ����� */
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");
	BehaviorComp = CreateDefaultSubobject<UCBehaviorComponent>("Behavior");

	
	// #1. Sight ����
	{
		Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");

		Sight->SightRadius = 900;
		Sight->LoseSightRadius = 1200;
		Sight->PeripheralVisionAngleDegrees = 90;
		Sight->SetMaxAge(2);

		Sight->DetectionByAffiliation.bDetectEnemies = true;
		Sight->DetectionByAffiliation.bDetectNeutrals = false;
		Sight->DetectionByAffiliation.bDetectFriendlies = false;
	}

	// #2. �켼�� ���� ����
	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
	BehaviorComp->OnStopLogic.BindUFunction(this, FName("ExecuteStopLogic"));
	BehaviorComp->OnBehaviorTypeChanged.AddDynamic(this, &ThisClass::OnBehaviorChanged);

	Status = Cast<UCStatusComponent>(GetPawn()->GetComponentByClass(UCStatusComponent::StaticClass()));
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FALSE_RETURN(bDrawDebug);

	//FVector center = OwnerEnemy->GetActorLocation();
	//center.Z -= AdjustCircleHeight;
	//DrawDebugCircle(GetWorld(), center, Sight->SightRadius, 300, FColor::Green, false, -1, 0, 0, FVector::RightVector, FVector::ForwardVector);
	//DrawDebugCircle(GetWorld(), center, MeleeActionRange, 300, FColor::Red, false, -1, 0, 0, FVector::RightVector, FVector::ForwardVector);
}

/** ó�� �����Ҷ� AIController ��ϵǾ��ִٸ� ���� */
void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// #1. TeamID ���
	OwnerEnemy = Cast<ACEnemy>(InPawn);
	SetGenericTeamId(OwnerEnemy->GetTeamID());

	// #2. Perception�� �ٲ�� �˷���
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);

	// #3. ���⼭ ����� Blackboard���ٰ� �÷��̾�� ����� Blackboard�����ͼ� ��� , Component���� ��Ͻ�����
	UseBlackboard(OwnerEnemy->GetBehaviorTree()->GetBlackboardAsset(), Blackboard);
	BehaviorComp->SetBlackboard(Blackboard);

	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
	Perception->OnPerceptionUpdated.Clear();
}


float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}

void ACAIController::OnBehaviorChanged(EBehaviorType InPrevType, EBehaviorType InNewType)
{
	if (InPrevType != InNewType)
	{
		if (InNewType == EBehaviorType::Patrol_BT)
		{
			if (Status)
			{
				Status->SetMovementSpeed(230.f);
			}
		}
		else
		{
			if (Status)
			{
				Status->SetMovementSpeed(600.f);
			}
		}
	}
}

/** Perception�� ������Ʈ �� ������ Perception�� Delegate�� ���� ����� */
void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	// #1. ������ Actor�� �߿��� Player�� ��������
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), actors);
	
	ACPlayer* Player = NULL;
	for (AActor* actor : actors)
	{
		Player = Cast<ACPlayer>(actor);

		if (!!Player)
			break;
	}

	Blackboard->SetValueAsObject("Player", Player);
}

void ACAIController::ExecuteStopLogic()
{
	GetBrainComponent()->StopLogic("Dead");
}
