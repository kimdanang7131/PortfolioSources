#pragma once

#include "CoreMinimal.h"
#include "Action/DoSkill/CDoSkill.h"
#include "CDoSkill_OneHand.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACDoSkill_OneHand : public ACDoSkill
{
	GENERATED_BODY()
	
public:
	ACDoSkill_OneHand() { PrimaryActorTick.bCanEverTick = true; index = 0;  }

protected:
	virtual void BeginPlay() { Super::BeginPlay(); }
	virtual void Tick(float DeltaTime) { Super::Tick(DeltaTime); }

public:
	virtual void DoSkill() { Super::DoSkill(); }
	virtual void End_DoSkill() { Super::End_DoSkill(); }
	virtual void End_Hold() { Super::End_Hold(); }
	virtual void DoSubSkill() { Super::DoSubSkill(); }
public:
	virtual void Activate() { Super::Activate(); }
	virtual void Deactivate() { Super::Deactivate(); }
};
