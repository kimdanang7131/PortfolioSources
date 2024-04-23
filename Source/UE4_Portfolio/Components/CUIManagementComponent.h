// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CUIManagementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCUIManagementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCUIManagementComponent();

protected:
	virtual void BeginPlay() override;

public:
	bool CheckUIMode() { return bIsVisible; }
	virtual void ClearUI() { }
	virtual void OpenUI() { }

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class ACharacter* OwnerCharacter;

	class ACPlayerController* PlayerController;
	class UCUserWidget_MainUI* MainUI;

protected:
	bool bIsVisible = false;
};
