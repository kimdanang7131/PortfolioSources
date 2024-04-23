#pragma once

#include "CoreMinimal.h"
#include "Action/DoSkill/KatanaSkill/CDoSkill_Katana.h"
#include "CDoSkill_Katana_03.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACDoSkill_Katana_03 : public ACDoSkill_Katana
{
	GENERATED_BODY()
	
public:
	ACDoSkill_Katana_03() { PrimaryActorTick.bCanEverTick = true; index = 2; }

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
