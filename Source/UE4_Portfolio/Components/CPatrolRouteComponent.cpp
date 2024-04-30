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

	// #1. �������� ����ϹǷ� ���� ���� �־�� ������ üũ
	if (PatrolRoute != nullptr)
	{
		Spline = PatrolRoute->GetSpline();

		// #2. ���� ��������
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

// #1. ������ ���� ����� Spline�� �����ͼ� ���
bool UCPatrolRouteComponent::GetDestination(FVector& OutLocation)
{
	// #2. �������� ��Ͼ��Ҽ��� �ֱ� ������ üũ �ʼ�
	if(Spline == nullptr)
		return false;

	// #3. PatrolRoute�� ������ ��ġ ��������
	OutLocation = Spline->GetLocationAtSplinePoint(routeIndex, ESplineCoordinateSpace::World);
	
	return true;
}

void UCPatrolRouteComponent::SetIndexToNext()
{
	if (Spline == nullptr)
		return;

	// #1. ClosedLoop == true�� �������� -> 0 1 2 0 1 2 �ݺ�
	if (Spline->IsClosedLoop())
	{
		if (routeIndex < routeMax)
		{
			routeIndex++;
		}
		else // �Ѿ�� �ٽ� ù��°��
		{
			routeIndex = 0;
		}
	}
	// #2. ClosedLoop == false�� �������� -> 0 1 2 1 0 1 2�ݺ�
	else
	{
		// #3. ������ġ�κ��� -2�� ������� 3���� �°Ŷ� 1�ΰ����� -2
		if (routeIndex >= routeMax)
		{
			routeIndex -= 2;
			converter   = -1;
		}
		// #4. ������ġ���� +2
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

