#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/CFItemDataTableBase.h"
#include "CUserWidget_InvenWindow.generated.h"

UCLASS()
class UE4_PORTFOLIO_API UCUserWidget_InvenWindow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	// ó�� Init�������� �������Ʈ���� ���� -> InvenSlotWidgetMap�� Slot���� ���������
	UFUNCTION(BlueprintCallable, Category = "InvenWindow")
		void SetInvenSlotWidget(const int32& InSlotIndex , class UCUserWidget_InvenSlot* InInvenSlotWidget);

	// �������Ʈ���� �ʱ�ȭ �Ҷ� ���� �Լ�
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InvenWindow")
		void CreateInvenSlots();

	// ���Կ� ������ �ִµ� ���� �̹��� ������ �� �̹����� �ְ� 99�� �̻��̸� �ٸ�ĭ�� �ִ� ���
	UFUNCTION(BlueprintCallable, Category = "InvenWindow")
		void AddFItemToInvenSlot(FItemDataTableBase InFItemData, bool bIsFItemQuantity = false, const int32& InQuantity = 1);

	// ����� �ִ� ��츸
	UFUNCTION(BlueprintCallable, Category = "InvenWindow")
		void AddFItemSelectedInvenSlot(FItemDataTableBase InFItemData , const int32& InSelectedIndex , class UCUserWidget_Description* InDescWidget);

	// �ٸ� �����۰� ����� �������� ��ȯ�� �� ���
	UFUNCTION(BlueprintCallable, Category = "InvenWindow")
		void SwapInvenSlot(const int32& InSwapIndex, const int32& InSwappedIndex);

	// InvenSlot�� ��� ��
	UFUNCTION(BlueprintCallable, Category = "InvenWindow")
		void ClearInvenSlot(const int32& InClearIndex);

	// TraderWindow ���� -> ��ȯ ���� ������ ������ ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = "InvenWindow")
		void ClearInventoryWindow();

	// ����ִ� ���� �� ���� ����� ���� ã�� �Լ�
	UFUNCTION(BlueprintCallable, Category = "InvenWindow")
		int32 FindNearestInvenSlot();

	// IsFromPlayerInven ���Ե� ����
	UFUNCTION(BlueprintCallable, Category = "InvenWindow")
		void SetPlayerInvenBool();

	// ������ ��ȯ + AddFItem
	UFUNCTION(BlueprintCallable, Category = "InvenWindow | Money")
		bool TradeItem(const int32& InSellerIndex , bool bIsDrag , bool bIsFromPlayerInven);

	// Debug
	void TestPrint();


public:
	// InvenComp�� ���� ���� Inventory �̹������� �����
	UFUNCTION(BlueprintCallable, Category = "InvenWindow | InvenComp")
		TArray<FItemDataTableBase> GetInvenWindowItems();

	UFUNCTION(BlueprintCallable, Category = "InvenWindow | InvenComp")
		int32 GetInvenMoney() { return money; }


	// �������Ʈ�� ���� -> ���� money�� �ؽ�Ʈ�� ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "InvenWindow | Money")
		void SetMoneyToText(const int32& InMoney);


	UFUNCTION(BlueprintCallable, Category = "InvenWindow | Money")
		void SetMoney(const int32& InMoney);

	UFUNCTION(BlueprintCallable, Category = "InvenWindow | Money")
		void AddMoney(const int32& InMoney);

	UFUNCTION(BlueprintCallable, Category = "InvenWindow | Money")
		bool SubMoney(const int32& InMoney);

	UFUNCTION(BlueprintCallable, Category = "InvenWindow | Money")
		bool CheckCanBuy(const int32& InMoney);


public:
	UPROPERTY(BlueprintReadOnly, Category = "InvenWindow | Variable")
		int32 InvenMaxCount = 25;

	UPROPERTY(BlueprintReadOnly, Category = "InvenWindow | Variable")
		int32 slotMaxCount = 99;

	UPROPERTY(BlueprintReadOnly, Category = "InvenWindow | Variable")
		int32 money = 0;


	// Buyer�� Seller�� Window ���� ����
	void SetBuyerAndSellerWindow(UCUserWidget_InvenWindow** Buyer, UCUserWidget_InvenWindow** Seller, bool bIsFromPlayerInven);

private:
	// ���� ������ ���� �ø� ��
	int32 AddSameItemQuantity(int32 InNum, FItemDataTableBase InFItemData);


	class UCUserWidget_InvenSlot* GetSlotWidget(const int32& InIndex) { return InvenSlotWidgetMap[InIndex]; }
	TArray<int32> GetSlotIndecies(UTexture2D* InTexture) { return InvenSlotIndexMap[InTexture]; }

private:
	TMap<UTexture2D* , TArray<int32>> InvenSlotIndexMap;
	TMap<int32, class UCUserWidget_InvenSlot*> InvenSlotWidgetMap;

};
