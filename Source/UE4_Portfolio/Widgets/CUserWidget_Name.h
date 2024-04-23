#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Name.generated.h"

UCLASS()
class UE4_PORTFOLIO_API UCUserWidget_Name : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void SetNameText(const FString& InName);

	UFUNCTION(BlueprintImplementableEvent)
		void SetTextColor(const FLinearColor& InColor);
};
