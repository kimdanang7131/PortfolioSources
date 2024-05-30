#pragma once

#include "CoreMinimal.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

class UE4_PORTFOLIO_API CSub
{
public:
	/** ���� �ε������� ���ܽ��Ѿ� �ϴ� �κ� �ϰ������� ��� ó���ϴ� �ڵ� */
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

	/** TakeDamge�� �ֱ� ���ؼ� ���ܻ�Ȳ ( ���� ������ ��Ȳ ) , ( LaunchAmount ó�� ) �� ���� �͵��� ��� ó�� */
	static bool CustomTakeDamage(const float& damageAmount , AActor* DamagedActor , AController* EventInstigator, AActor* DamageCauser )
	{
		// #1. StateComp�� �ְ� ������ ���� �ƴҶ��� ����
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
			// �ΰ��� �ƴҶ� ���� ó���ؾ���
		}
		return false;
	}



	/** ������ FRotator�κ��� Forward , Up , Right�� ���͸� �̾ƾ� �ؼ� ���� ���� */
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



	/** ���� �迭�� �޾Ƽ� BaseActor�κ��� ���� �Ÿ��� �� ���͸� ã�� �ڵ� */
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

	/** ���� �迭�� �޾Ƽ� BaseActor�κ��� ���� �Ÿ��� ����� ���͸� ã�� �ڵ� */
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