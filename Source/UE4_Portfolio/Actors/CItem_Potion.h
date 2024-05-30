#pragma once

#include "CoreMinimal.h"
#include "Actors/CItem.h"
#include "Structs/CFItemDataTableBase.h"
#include "CItem_Potion.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACItem_Potion : public ACItem
{
	GENERATED_BODY()

protected:
	ACItem_Potion();
public:
	virtual void AcivateItem(FItemDataTableBase FItem) override;

	UPROPERTY(EditAnywhere, Category = "Montage")
		class UAnimMontage* DrinkMontage;

	UPROPERTY(EditAnywhere)
		float amount = 1.f;
};
