#pragma once

#include "CoreMinimal.h"
#include "Action/DoSkill/CDoSkill.h"
#include "CDoSkill_TwoHand.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACDoSkill_TwoHand : public ACDoSkill
{
	GENERATED_BODY()
	
public:
	ACDoSkill_TwoHand() { PrimaryActorTick.bCanEverTick = true; bCanRotate = true; rInterpSpeed = 3.f; }

protected:
	virtual void BeginPlay() { Super::BeginPlay(); }
	virtual void Tick(float DeltaTime) { Super::Tick(DeltaTime); }

public:
	virtual void DoSkill() { Super::DoSkill(); }
	virtual void End_DoSkill() { Super::End_DoSkill(); }
	virtual void End_Hold() { Super::End_Hold(); }
	virtual void DoSubSkill() { Super::DoSubSkill(); }

	virtual void OnAttackByTimer() { Super::OnAttackByTimer(); }
public:
	virtual void Activate() { Super::Activate(); }
	virtual void Deactivate() { Super::Deactivate(); }
};
