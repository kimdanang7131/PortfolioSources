#include "Components/CPatrolRouteComponent.h"
#include "Global.h"

#include "Components/SplineComponent.h"
#include "BehaviorTree/CPatrolRoute.h"

UCPatrolRouteComponent::UCPatrolRouteComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCPatrolRouteComponent::BeginPlay()
{
	Super::BeginPlay();

	// #1. 레벨에서 등록하므로 없을 수도 있어어 무조건 체크
	if (PatrolRoute != nullptr)
	{
		Spline = PatrolRoute->GetSpline();

		// #2. 개수 가져오기
		if (Spline)
		{
			routeMax = Spline->GetNumberOfSplinePoints();
		}
	}
		
}


void UCPatrolRouteComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

// #1. 레벨에 따로 등록한 Spline을 가져와서 사용
bool UCPatrolRouteComponent::GetDestination(FVector& OutLocation)
{
	// #2. 레벨에서 등록안할수도 있기 때문에 체크 필수
	if(Spline == nullptr)
		return false;

	// #3. PatrolRoute중 현재의 위치 가져오기
	OutLocation = Spline->GetLocationAtSplinePoint(routeIndex, ESplineCoordinateSpace::World);
	
	return true;
}

void UCPatrolRouteComponent::SetIndexToNext()
{
	if (Spline == nullptr)
		return;

	// #1. ClosedLoop == true면 닫혀있음 -> 0 1 2 0 1 2 반복
	if (Spline->IsClosedLoop())
	{
		if (routeIndex < routeMax)
		{
			routeIndex++;
		}
		else // 넘어가면 다시 첫번째로
		{
			routeIndex = 0;
		}
	}
	// #2. ClosedLoop == false면 열려있음 -> 0 1 2 1 0 1 2반복
	else
	{
		// #3. 현재위치로부터 -2를 해줘야함 3까지 온거라서 1로가려면 -2
		if (routeIndex >= routeMax)
		{
			routeIndex -= 2;
			converter   = -1;
		}
		// #4. 현재위치부터 +2
		else if (routeIndex < 0)
		{
			routeIndex += 2;
			converter   = 1;
		}
		else
		{
			routeIndex += converter;
		}
	}

}

