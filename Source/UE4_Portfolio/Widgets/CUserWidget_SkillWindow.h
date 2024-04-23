#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_SkillWindow.generated.h"

UCLASS()
class UE4_PORTFOLIO_API UCUserWidget_SkillWindow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void CreateSkillSlots(class UDataTable* SkillDT);

	UFUNCTION(BlueprintCallable)
		void UseSkillItem(UTexture2D* InTexture);

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<class UCUserWidget_SkillSlot*> SkillSlotWidgets;
}; 
