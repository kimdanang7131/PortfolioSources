#pragma once

#include "CoreMinimal.h"
#include "Action/DoSkill/TwoHandSkill/CDoSkill_TwoHand.h"
#include "CDoSkill_TwoHand_02.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACDoSkill_TwoHand_02 : public ACDoSkill_TwoHand
{
	GENERATED_BODY()
	
public:
	ACDoSkill_TwoHand_02() { PrimaryActorTick.bCanEverTick = true; index = 1; }

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


public:
	virtual void DoSkill() {}
	virtual void End_Hold() {}

public:
	virtual void Activate() override;
	virtual void Deactivate() override;
};
