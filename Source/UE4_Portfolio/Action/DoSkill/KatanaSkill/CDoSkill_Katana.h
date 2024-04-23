#pragma once

#include "CoreMinimal.h"
#include "Action/DoSkill/CDoSkill.h"
#include "CDoSkill_Katana.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACDoSkill_Katana : public ACDoSkill
{
	GENERATED_BODY()
	
public:
	ACDoSkill_Katana() { PrimaryActorTick.bCanEverTick = true; }

protected:
	virtual void BeginPlay() { Super::BeginPlay(); bCanRotate = true; rInterpSpeed = 3.f; }
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
