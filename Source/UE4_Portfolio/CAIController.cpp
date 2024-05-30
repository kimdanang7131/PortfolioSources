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

	/** Blackboard와 Perception은 기본적으로 내장되어있음 */
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");
	BehaviorComp = CreateDefaultSubobject<UCBehaviorComponent>("Behavior");

	
	// #1. Sight 설정
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

	// #2. 우세한 감각 설정
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

/** 처음 시작할때 AIController 등록되어있다면 빙의 */
void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// #1. TeamID 등록
	OwnerEnemy = Cast<ACEnemy>(InPawn);
	SetGenericTeamId(OwnerEnemy->GetTeamID());

	// #2. Perception이 바뀌면 알려줌
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);

	// #3. 여기서 사용할 Blackboard에다가 플레이어에서 등록한 Blackboard가져와서 등록 , Component에도 등록시켜줌
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

/** Perception이 업데이트 될 때마다 Perception의 Delegate를 통해 실행됨 */
void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	// #1. 감지된 Actor들 중에서 Player만 가져오기
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
