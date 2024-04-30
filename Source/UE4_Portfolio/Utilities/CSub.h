#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"

class UE4_PORTFOLIO_API CSub
{
public:
	static bool CheckActorBeginOverlapped(AActor* OwnerCharacter, AActor* OtherActor)
	{
		if (OwnerCharacter == OtherActor)
			return true;
		if (OwnerCharacter->GetClass() == OtherActor->GetClass())
			return true;
		if (OwnerCharacter->GetClass() == OtherActor->GetOwner()->GetClass())
			return true;

		return false;
	}

	static FVector GetCustomForwardVector(AActor* Actor)
	{
		FRotator rotator = Actor->GetActorRotation();
		return FQuat(rotator).GetForwardVector();
	}
	static FVector GetCustomRightVector(AActor* Actor)
	{
		FRotator rotator = Actor->GetActorRotation();
		return FQuat(rotator).GetRightVector();
	}
	static FVector GetCustomUpVector(AActor* Actor)
	{
		FRotator rotator = Actor->GetActorRotation();
		return FQuat(rotator).GetUpVector();
	}


	static AActor* FindFarthestActor(AActor* BaseActor , const TArray<AActor*>& InActors)
	{
		if (!InActors.Num())
		{
			CLog::Print("CSub : FindFarthestActor null");
			return nullptr;
		}


		FVector BaseLocation = BaseActor->GetActorLocation();

		float maxDistSquared = -1.f;

		AActor* FarthestActor = nullptr;

		for (int32 i = 0; i < InActors.Num(); i++)
		{
			float DistSquared = FVector::DistSquared(InActors[i]->GetActorLocation(), BaseLocation);
			// 최대거리 저장 및 가장 먼 액터 저장
			if (maxDistSquared < DistSquared)
			{
				maxDistSquared = DistSquared;
				FarthestActor = InActors[i];
			}
		}

		return FarthestActor;
	}

	static AActor* FindNearestActor(AActor* BaseActor, const TArray<AActor*>& InActors)
	{
		if (!InActors.Num())
		{
			CLog::Print("CSub : FindNearestActor null");
			return nullptr;
		}


		FVector BaseLocation = BaseActor->GetActorLocation();

		float minDistSquared = FLT_MAX;

		AActor* NearestActor = nullptr;

		for (int32 i = 0; i < InActors.Num(); i++)
		{
			float DistSquared = FVector::DistSquared(InActors[i]->GetActorLocation(), BaseLocation);
			// 최대거리 저장 및 가장 먼 액터 저장
			if (minDistSquared > DistSquared)
			{
				minDistSquared = DistSquared;
				NearestActor = InActors[i];
			}
		}

		return NearestActor;
	}
};



//static void Delay(const float& delayTime)
//{
//	FTimerHandle Delay;
//	float DelayTime = 0.25f;

//	GetWorld()->GetTimerManager().SetTimer(Delay, FTimerDelegate::CreateLambda([&]()
//		{
//			// 코드 구현
//			Destroy();
//			// TimerHandle 초기화
//			GetWorld()->GetTimerManager().ClearTimer(Delay);
//		}), delayTime, false);	// 반복하려면 false를 true로 변경
//}