#include "BehaviorTree/CPatrolRoute.h"
#include "Global.h"

// Sets default values
ACPatrolRoute::ACPatrolRoute()
{
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}
