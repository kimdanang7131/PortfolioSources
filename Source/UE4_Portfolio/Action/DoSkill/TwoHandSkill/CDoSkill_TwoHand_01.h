#pragma once

#include "CoreMinimal.h"
#include "Action/DoSkill/TwoHandSkill/CDoSkill_TwoHand.h"
#include "CDoSkill_TwoHand_01.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACDoSkill_TwoHand_01 : public ACDoSkill_TwoHand
{
	GENERATED_BODY()
public:
	ACDoSkill_TwoHand_01() { PrimaryActorTick.bCanEverTick = true;  index = 0; coolTime = 12.f; }
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
public:
	virtual void DoSkill() {}
	virtual void End_Hold() {}

	virtual void OnAttackByTimer() override;
	virtual void OffAttackByTimer() override;

public:
	virtual void Activate() override;
	virtual void Deactivate() override;
};
