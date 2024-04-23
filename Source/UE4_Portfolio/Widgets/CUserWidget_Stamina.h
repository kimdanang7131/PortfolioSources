#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Stamina.generated.h"

UCLASS()
class UE4_PORTFOLIO_API UCUserWidget_Stamina : public UUserWidget
{
	GENERATED_BODY()
		
public:
	UFUNCTION(BlueprintImplementableEvent)
		void Update(float Stamina, float MaxStamina);
};
