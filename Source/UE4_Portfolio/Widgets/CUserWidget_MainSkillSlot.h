#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/CFSkillDataTableBase.h"
#include "CUserWidget_MainSkillSlot.generated.h"

UCLASS()
class UE4_PORTFOLIO_API UCUserWidget_MainSkillSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE UTexture2D* GetSkillTexture() { return SkillTexture; }

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "MainSkillSlot")
		void SetSkillTexture(UTexture2D* InSkillTexture);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "MainSkillSlot")
		void SetFSkillData(FSkillDataTable InFSkillData);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "MainSkillSlot")
		void SetDescription(class UCUserWidget_Description* InSkillDescWidget);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "MainSkillSlot")
		void SetSlotOpacity();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SkillSlot")
		void SetSkillActivated(bool bIsActivated);

public:
	UFUNCTION(BlueprintCallable, Category = "MainSkillSlot")
		void SwapData(UCUserWidget_MainSkillSlot* SwapSlot);

	UFUNCTION(BlueprintCallable, Category = "MainSkillSlot")
		bool CheckSlotEmpty();

	UFUNCTION(BlueprintCallable, Category = "MainSkillSlot")
		void ClearSlot();

public:
	UPROPERTY(BlueprintReadWrite, Category = "MainSkillSlot")
		int32 UseSkillIndex;

	UPROPERTY(BlueprintReadWrite, Category = "MainSkillSlot")
		UTexture2D* SkillTexture;

	UPROPERTY(BlueprintReadWrite, Category = "MainSkillSlot")
		FSkillDataTable FSkillData;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "MainSkillSlot")
		TSubclassOf<class UCUserWidget_Description> SkillDescClass;

	UPROPERTY(BlueprintReadWrite, Category = "MainSkillSlot")
		class UCUserWidget_Description* SkillDescWidget;

public:
	void SwapDatas(UTexture2D* InSkillTexture, FSkillDataTable InFSkillData, class UCUserWidget_Description* InSkillDescWidget);

};
