#pragma once

#include "CoreMinimal.h"
#include "Action/DoSkill/OneHandSkill/CDoSkill_OneHand.h"
#include "CDoSkill_OneHand_01.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACDoSkill_OneHand_01 : public ACDoSkill_OneHand
{
	GENERATED_BODY()

		
public:
	ACDoSkill_OneHand_01();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


public:
	virtual void DoSkill() {}
	virtual void End_Hold() {}

	virtual void DoSubSkill() override;


	//UFUNCTION(BlueprintCallable)
	//	void TakeDamage();

public:
	virtual void Activate() override;
	virtual void Deactivate() override;

private:
	enum { INTERVAL = 50 };

protected:
	int starCount = 1;

	TSubclassOf<class ACActor_Sub_Star> starClass;
};
