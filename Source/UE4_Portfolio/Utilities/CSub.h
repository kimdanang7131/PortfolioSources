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
			// �ִ�Ÿ� ���� �� ���� �� ���� ����
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
			// �ִ�Ÿ� ���� �� ���� �� ���� ����
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
//			// �ڵ� ����
//			Destroy();
//			// TimerHandle �ʱ�ȭ
//			GetWorld()->GetTimerManager().ClearTimer(Delay);
//		}), delayTime, false);	// �ݺ��Ϸ��� false�� true�� ����
//}