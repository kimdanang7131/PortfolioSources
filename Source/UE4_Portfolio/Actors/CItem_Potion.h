#pragma once

#include "CoreMinimal.h"
#include "Actors/CItem.h"
#include "Structs/CFItemDataTableBase.h"
#include "CItem_Potion.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACItem_Potion : public ACItem
{
	GENERATED_BODY()
	
public:
	virtual void AcivateItem(FItemDataTableBase FItem) override;

protected:
	float amount = 1.f;
};
