#pragma once

#include "CoreMinimal.h"
#include "Action/DoSkill/DualBladeSkill/CDoSkill_DualBlade.h"
#include "CDoSkill_DualBlade_03.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACDoSkill_DualBlade_03 : public ACDoSkill_DualBlade
{
	GENERATED_BODY()
	
public:
	ACDoSkill_DualBlade_03() { PrimaryActorTick.bCanEverTick = true; index = 2; coolTime = 20.f; }

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
