#pragma once

#include "CoreMinimal.h"
#include "Action/DoSkill/DualBladeSkill/CDoSkill_DualBlade.h"
#include "CDoSkill_DualBlade_02.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACDoSkill_DualBlade_02 : public ACDoSkill_DualBlade
{
	GENERATED_BODY()
	
public:
	ACDoSkill_DualBlade_02() { PrimaryActorTick.bCanEverTick = true; index = 1; }
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void DoSkill() {}
	virtual void End_Hold() {}
	
	virtual void DoSubSkill() override;
public:
	virtual void Activate() override;
	virtual void Deactivate() override;
private:
	float converter = -1.f;
	float speed     = 5.f;
};
