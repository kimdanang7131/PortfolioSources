#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Engine/DataTable.h"
#include "Structs/CFItemDataTableBase.h"
#include "Components/UIComponents/CUIManagementComponent.h"
#include "CInventoryComponent.generated.h"

/** InventoryComponent에 DataTable을 넣은걸 이용 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCInventoryComponent : public UCUIManagementComponent
{
	GENERATED_BODY()
public:	
	UCInventoryComponent();
protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE TArray<FItemDataTableBase> GetFItemDatas() { return InvenFItems; }
	FORCEINLINE int32 GetMoney() { return money; }

public:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCStateComponent* State;

	/** ItemDataTable을 통해서 Player만 UI에 등록 */
	UPROPERTY(EditDefaultsOnly)
		UDataTable* ItemDT;


	/** UCUIManagementComponent 재정의 */
public:
	virtual void OpenUI() override;
	virtual void ClearUI() override;

public:
	/** 처음 DataTable을 통해서 UI생성 */
	void InitFItemDataTable();

	/** CPlayer에서 껏다키고 할 함수 */
	void ToggleInventoryWindow();

	/** Trader 영역에 들어오면 Delegate를 통해 Player에서 Trader Window를 Open시킴 */
	void OpenTraderWindow(const TArray<FItemDataTableBase>& InTraderFItems, const int32& InMoney);
	/** Trader 영역을 벗어나면 모든 UI를 종료 , GameMode로 변경 , Trader Window를 Clear, Player에서 델리게이트를 통해 UpdateTraderInvenDatas Trader에게 전달 */
	void CloseTraderWindow();

	/** Trader와 거래 끝나면 -> Player에서 델리게이트를 통해 UpdateTraderInvenDatas Trader에게 전달 */
	void UpdateTraderInvenDatas(TArray<FItemDataTableBase> InInvenFItems, const int32& InMoney);
	/** Trader와 거래 끝나면 -> Player인벤토리 업데이트 */
	void UpdatePlayerInvenDatas();

public:
	/** Trader와 거래 끝나면 -> Trader의 Money와 인베토리 업데이트 넘겨줄 정보들 */
	TArray<FItemDataTableBase> GetTraderInvenDatas();
	int32 GetTraderMoney();

	/** 아이템 사용 */
	void UseSpawnedItem(TSubclassOf<class ACItem> InUseItem ,FItemDataTableBase FItem);

	/** Player가 1234 입력해서 아이템 사용 */
	void UseInvenSlotItem(const int32& InSlotInvenIndex);

	// Test
	void AddFItemToUI();
	void AddFItemToUI2();
	void TestPrint();
private:
	TMap<TSubclassOf<class ACItem>, class ACItem*> SpawnItems;
	TArray<FItemDataTableBase> InvenFItems;
	int32 money = 0;
};
