#pragma once

#include "CoreMinimal.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"

//#include "Kismet/GameplayStatics.h"
//#include "Kismet/KismetMathLibrary.h"
//#include "Kismet/KismetSystemLibrary.h"

class UE4_PORTFOLIO_API CHelpers
{
public:
	template<typename T>
	static void MyFObjectFinder(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succeeded()");

		*OutObject = asset.Object;
	}

	template<typename T>
	static void MyLoadObjectDynamic(T** OutObject, FString InPath)
	{
		T* obj = Cast<T>(StaticLoadObject(T::StaticClass(), NULL, *InPath));
		verifyf(!!obj, L"!!asset");
		
		*OutObject = obj;
	}

	/////////////////////////



	template<typename T>
	static void MyFClassFinder(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succeeded()");

		*OutClass = asset.Class;
	}


	//////////////////////////////////////////////////


	template<typename T>
	static void CreateComponent(AActor* InActor, T** InComponent, FName InName, USceneComponent* InParent = NULL)
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);

		if (!!InParent)
		{
			(*InComponent)->SetupAttachment(InParent);

			return;
		}

		InActor->SetRootComponent((*InComponent));
	}

	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** InComponent, FName InName)
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);
	}

	/////////////////////////


	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}

	template<typename T>
	static void MyGetAllActorsOfClass(class UWorld* InWorld, TArray<T *>& OutActors)
	{
		OutActors.Empty();

		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(InWorld, T::StaticClass(), actors);
		
		for (AActor* actor : actors)
			OutActors.Add(Cast<T>(actor));
	}

	template<typename T>
	static T* MySpawnActor(TSubclassOf<T> ActorClass, AActor* Owner , FTransform spawnTransform)
	{
		if (!Owner) { return nullptr; }

		FActorSpawnParameters SpawnParams;

		SpawnParams.Owner = Owner;

		APawn* Pawn = Cast<APawn>(Owner);
		if (!Pawn) { return nullptr; }

		SpawnParams.Instigator = Pawn;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		T* Actor = Owner->GetWorld()->SpawnActor<T>(ActorClass, spawnTransform, SpawnParams);

		if (!Actor) { return nullptr; }

		return Actor;
	}
	
	template<typename T>
	static T* MySpawnActorBPObject(UObject* SpawnObject, AActor* Owner)
	{
		// #1. Blueprint인지 확인하는 작업 -> Blueprint인것만 다이나믹캐스트하여 RTTI로 인하여 맞는지 확인
		UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnObject);
		if (!GeneratedBP)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
			return nullptr;
		}

		// #2. SpawnActor를 하기위해서 Class를 가져오는데 Class를 가져오는지 확인하는 작업
		UClass* SpawnClass = GeneratedBP->StaticClass();

		if (!SpawnClass)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
		}

		// #3. SpawnActor 하는 과정 중요한점은 SpawnActor의 반환값이 T*이기 때문에 T*로 저장
		FActorSpawnParameters spawnParameters;
		spawnParameters.Owner = Owner;

		APawn* Pawn = Cast<APawn>(Owner);
		if (!Pawn) { return nullptr; }
		spawnParameters.Instigator = Pawn;
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		T* Actor = Owner->GetWorld()->SpawnActor<T>(GeneratedBP->GeneratedClass, Owner->GetTransform(), spawnParameters);
		if (!Actor) { return nullptr; }

		return Actor;
	}


};
