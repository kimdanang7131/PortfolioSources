#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/CFSkillDataTableBase.h"
#include "CUserWidget_SkillSlot.generated.h"

UCLASS()
class UE4_PORTFOLIO_API UCUserWidget_SkillSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE UTexture2D* GetSkillTexture() { return SkillTexture; }

	UFUNCTION(BlueprintCallable, Category = "SkillSlot")
		void SetConstructEvent(int32 InIndex, FSkillDataTable InFSkillData);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SkillSlot")
		void SetSkillTexture(UTexture2D* InSkillTexture);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SkillSlot")
		void SetFSkillData(FSkillDataTable InFSkillData);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SkillSlot")
		void SetDescription(class UCUserWidget_Description* InSkillDescWidget);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SkillSlot")
		void SetDescriptionToCanvas(class UCUserWidget_Description* InSkillDescWidget);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SkillSlot")
		void SetSkillActivated(bool bIsActivated);



public:
	UPROPERTY(BlueprintReadWrite, Category = "SkillSlot")
		int32 Index;

	UPROPERTY(BlueprintReadWrite, Category = "SkillSlot")
		UTexture2D* SkillTexture;

	UPROPERTY(BlueprintReadWrite, Category = "SkillSlot")
		FSkillDataTable FSkillData;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SkillSlot")
		TSubclassOf<class UCUserWidget_Description> SkillDescClass;

	UPROPERTY(BlueprintReadWrite, Category = "SkillSlot")
		class UCUserWidget_Description* SkillDescWidget;

	//UFUNCTION()
	//	void OnDragDetected(UDragDropOperation* DragDropOperation);

	//UFUNCTION()
	//	void OnDrop(FGeometry MyGeometry, const FDragDropEvent& DragDropEvent);
};
