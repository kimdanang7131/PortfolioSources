#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/CFItemDataTableBase.h"
#include "CUserWidget_InvenSlot.generated.h"

UCLASS()
class UE4_PORTFOLIO_API UCUserWidget_InvenSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;


	FORCEINLINE int32 GetSlotIndex() { return Index; }
	FORCEINLINE int32 GetItemQuantity() { return Quantity; }
	FORCEINLINE UTexture2D* GetItemTexture() { return ItemTexture; }
	FORCEINLINE FItemDataTableBase GetFItem() { return FItemData; }

public:
	UFUNCTION(BlueprintCallable, Category = "InvenSlot")
		void SetConstructEvent(const int32& InIndex);

	UFUNCTION(BlueprintCallable, Category = "InvenSlot")
		void SetSlotData(const FItemDataTableBase& InFItemData, const int32& InQuantity);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InvenSlot")
		void SetItemTexture(UTexture2D* InItemTexture);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InvenSlot")
		void SetItemQuantity(int32 InQuantity);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InvenSlot")
		void SetFItemData(const FItemDataTableBase& InFItemData);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InvenSlot")
		void SetDescription(class UCUserWidget_Description* InItemDescWidget);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InvenSlot")
		void SetDescriptionToCanvas(class UCUserWidget_Description* InItemDescWidget);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InvenSlot")
		void SetSlotOpacity();

	UFUNCTION(BlueprintCallable, Category = "InvenSlot")
		void UseSlotItem();


	UFUNCTION(BlueprintCallable, Category = "InvenSlot")
		void SetFItemQuantity(const int32& InQuantity);

public:

	UFUNCTION(BlueprintCallable, Category = "InvenSlot")
		bool CheckSameItem(const UTexture2D* InTexture);

	UFUNCTION(BlueprintCallable, Category = "InvenSlot")
		void SwapData(const FItemDataTableBase& InFItemData , class UCUserWidget_Description* InItemDescWidget);

	UFUNCTION(BlueprintCallable, Category = "InvenSlot")
		bool CheckSlotEmpty();

	UFUNCTION(BlueprintCallable, Category = "InvenSlot")
		void ClearSlot();


public:
	UPROPERTY(BlueprintReadWrite, Category = "InvenSlot")
		int32 Index;

	UPROPERTY(BlueprintReadWrite, Category = "InvenSlot")
		bool bIsPlayerInven;

	UPROPERTY(BlueprintReadWrite, Category = "InvenSlot")
		int32 Quantity;

	UPROPERTY(BlueprintReadWrite, Category = "InvenSlot")
		UTexture2D* ItemTexture = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "InvenSlot")
		FItemDataTableBase FItemData;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "InvenSlot")
		TSubclassOf<class UCUserWidget_Description> ItemDescClass;

	UPROPERTY(BlueprintReadWrite, Category = "InvenSlot")
		class UCUserWidget_Description* ItemDescWidget;
};
