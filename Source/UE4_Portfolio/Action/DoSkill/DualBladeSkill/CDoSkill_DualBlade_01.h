#pragma once

#include "CoreMinimal.h"
#include "Action/DoSkill/DualBladeSkill/CDoSkill_DualBlade.h"
#include "CDoSkill_DualBlade_01.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACDoSkill_DualBlade_01 : public ACDoSkill_DualBlade
{
	GENERATED_BODY()
	
public:
	ACDoSkill_DualBlade_01() { PrimaryActorTick.bCanEverTick = true; index = 0;}

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
