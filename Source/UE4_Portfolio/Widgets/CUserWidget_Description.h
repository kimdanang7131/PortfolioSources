#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Description.generated.h"

UCLASS()
class UE4_PORTFOLIO_API UCUserWidget_Description : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Description")
		void SetDescription(const FText& InDesc);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Description")
		void SetName(const FText& InName);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Description")
		void SetDescriptionVisibility(bool bIsVisible);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Description")
		void SetNameVisibility(bool bIsVisible);

public:
	UPROPERTY(BlueprintReadOnly, Category = "Description")
		FText Desc;

	UPROPERTY(BlueprintReadOnly, Category = "Description")
		FText Name;
};
