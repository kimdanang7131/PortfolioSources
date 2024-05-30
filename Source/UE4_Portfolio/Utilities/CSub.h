#pragma once

#include "CoreMinimal.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

class UE4_PORTFOLIO_API CSub
{
public:
	/** 적과 부딪혔을때 예외시켜야 하는 부분 일괄적으로 모아 처리하는 코드 */
	static bool CheckActorBeginOverlapped(AActor* OwnerCharacter, AActor* OtherActor)
	{
		if (OwnerCharacter == OtherActor)
			return true;
		if (OwnerCharacter->GetClass() == OtherActor->GetClass())
			return true;
		if (OtherActor->GetOwner())
		{
			if (OwnerCharacter->GetClass() == OtherActor->GetOwner()->GetClass())
				return true;
		}


		return false;
	}

	/** TakeDamge를 주기 위해서 예외상황 ( 적이 구르는 상황 ) , ( LaunchAmount 처리 ) 에 대한 것들을 모아 처리 */
	static bool CustomTakeDamage(const float& damageAmount , AActor* DamagedActor , AController* EventInstigator, AActor* DamageCauser )
	{
		// #1. StateComp가 있고 구르기 중이 아닐때만 진행
		UCStateComponent* TargetState = Cast<UCStateComponent>(DamagedActor->GetComponentByClass(UCStateComponent::StaticClass()));
		if (TargetState)
		{
			if (TargetState->IsDodgeMode() == false)
			{
				DamagedActor->TakeDamage(damageAmount, FDamageEvent(), EventInstigator, DamageCauser);
				
				return true;
			}
		}
		else
		{
			// 인간형 아닐때 따로 처리해야함
		}
		return false;
	}



	/** 일일이 FRotator로부터 Forward , Up , Right의 벡터를 뽑아야 해서 각각 정리 */
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



	/** 액터 배열을 받아서 BaseActor로부터 가장 거리가 먼 액터를 찾는 코드 */
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

	/** 액터 배열을 받아서 BaseActor로부터 가장 거리가 가까운 액터를 찾는 코드 */
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