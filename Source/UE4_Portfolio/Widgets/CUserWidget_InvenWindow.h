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

	// 처음 Init과정에서 블루프린트에서 설정 -> InvenSlotWidgetMap에 Slot마다 연결시켜줌
	UFUNCTION(BlueprintCallable, Category = "InvenWindow")
		void SetInvenSlotWidget(const int32& InSlotIndex , class UCUserWidget_InvenSlot* InInvenSlotWidget);

	// 블루프린트에서 초기화 할때 쓰는 함수
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InvenWindow")
		void CreateInvenSlots();

	// 슬롯에 아이템 넣는데 같은 이미지 있으면 그 이미지에 넣고 99개 이상이면 다른칸에 넣는 기능
	UFUNCTION(BlueprintCallable, Category = "InvenWindow")
		void AddFItemToInvenSlot(FItemDataTableBase InFItemData, bool bIsFItemQuantity = false, const int32& InQuantity = 1);

	// 빈곳에 넣는 경우만
	UFUNCTION(BlueprintCallable, Category = "InvenWindow")
		void AddFItemSelectedInvenSlot(FItemDataTableBase InFItemData , const int32& InSelectedIndex , class UCUserWidget_Description* InDescWidget);

	// 다른 아이템과 드롭한 아이템을 교환할 때 사용
	UFUNCTION(BlueprintCallable, Category = "InvenWindow")
		void SwapInvenSlot(const int32& InSwapIndex, const int32& InSwappedIndex);

	// InvenSlot을 비울 때
	UFUNCTION(BlueprintCallable, Category = "InvenWindow")
		void ClearInvenSlot(const int32& InClearIndex);

	// TraderWindow 전용 -> 교환 구역 밖으로 나가면 비우는 함수
	UFUNCTION(BlueprintCallable, Category = "InvenWindow")
		void ClearInventoryWindow();

	// 비어있는 슬롯 중 가장 가까운 곳을 찾는 함수
	UFUNCTION(BlueprintCallable, Category = "InvenWindow")
		int32 FindNearestInvenSlot();

	// IsFromPlayerInven 슬롯들 세팅
	UFUNCTION(BlueprintCallable, Category = "InvenWindow")
		void SetPlayerInvenBool();

	// 아이템 교환 + AddFItem
	UFUNCTION(BlueprintCallable, Category = "InvenWindow | Money")
		bool TradeItem(const int32& InSellerIndex , bool bIsDrag , bool bIsFromPlayerInven);

	// Debug
	void TestPrint();


public:
	// InvenComp로 보낼 현재 Inventory 이미지들의 결과들
	UFUNCTION(BlueprintCallable, Category = "InvenWindow | InvenComp")
		TArray<FItemDataTableBase> GetInvenWindowItems();

	UFUNCTION(BlueprintCallable, Category = "InvenWindow | InvenComp")
		int32 GetInvenMoney() { return money; }


	// 블루프린트와 연결 -> 현재 money를 텍스트로 띄우기
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


	// Buyer와 Seller의 Window 각각 지정
	void SetBuyerAndSellerWindow(UCUserWidget_InvenWindow** Buyer, UCUserWidget_InvenWindow** Seller, bool bIsFromPlayerInven);

private:
	// 같은 아이템 개수 늘릴 때
	int32 AddSameItemQuantity(int32 InNum, FItemDataTableBase InFItemData);


	class UCUserWidget_InvenSlot* GetSlotWidget(const int32& InIndex) { return InvenSlotWidgetMap[InIndex]; }
	TArray<int32> GetSlotIndecies(UTexture2D* InTexture) { return InvenSlotIndexMap[InTexture]; }

private:
	TMap<UTexture2D* , TArray<int32>> InvenSlotIndexMap;
	TMap<int32, class UCUserWidget_InvenSlot*> InvenSlotWidgetMap;

};
