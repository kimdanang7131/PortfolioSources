#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Engine/DataTable.h"
#include "Structs/CFItemDataTableBase.h"
#include "Components/UIComponents/CUIManagementComponent.h"
#include "CInventoryComponent.generated.h"

/** InventoryComponent�� DataTable�� ������ �̿� */
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

	/** ItemDataTable�� ���ؼ� Player�� UI�� ��� */
	UPROPERTY(EditDefaultsOnly)
		UDataTable* ItemDT;


	/** UCUIManagementComponent ������ */
public:
	virtual void OpenUI() override;
	virtual void ClearUI() override;

public:
	/** ó�� DataTable�� ���ؼ� UI���� */
	void InitFItemDataTable();

	/** CPlayer���� ����Ű�� �� �Լ� */
	void ToggleInventoryWindow();

	/** Trader ������ ������ Delegate�� ���� Player���� Trader Window�� Open��Ŵ */
	void OpenTraderWindow(const TArray<FItemDataTableBase>& InTraderFItems, const int32& InMoney);
	/** Trader ������ ����� ��� UI�� ���� , GameMode�� ���� , Trader Window�� Clear, Player���� ��������Ʈ�� ���� UpdateTraderInvenDatas Trader���� ���� */
	void CloseTraderWindow();

	/** Trader�� �ŷ� ������ -> Player���� ��������Ʈ�� ���� UpdateTraderInvenDatas Trader���� ���� */
	void UpdateTraderInvenDatas(TArray<FItemDataTableBase> InInvenFItems, const int32& InMoney);
	/** Trader�� �ŷ� ������ -> Player�κ��丮 ������Ʈ */
	void UpdatePlayerInvenDatas();

public:
	/** Trader�� �ŷ� ������ -> Trader�� Money�� �κ��丮 ������Ʈ �Ѱ��� ������ */
	TArray<FItemDataTableBase> GetTraderInvenDatas();
	int32 GetTraderMoney();

	/** ������ ��� */
	void UseSpawnedItem(TSubclassOf<class ACItem> InUseItem ,FItemDataTableBase FItem);

	/** Player�� 1234 �Է��ؼ� ������ ��� */
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
