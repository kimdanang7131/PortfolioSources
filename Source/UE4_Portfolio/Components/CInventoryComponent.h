#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Components/CUIManagementComponent.h"
#include "Structs/CFItemDataTableBase.h"
#include "CInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCInventoryComponent : public UCUIManagementComponent
{
	GENERATED_BODY()

public:	
	UCInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	TArray<FItemDataTableBase> GetTraderInvenDatas();
	int32 GetTraderMoney();

public:
	void ToggleInventoryWindow();

	int32 GetMoney() { return money; }


	void UpdateTraderInvenDatas(TArray<FItemDataTableBase> InInvenFItems, const int32& InMoney);
	void UpdatePlayerInvenDatas();


	void AddFItemToUI();
	void AddFItemToUI2();
	void TestPrint();

	virtual void OpenUI() override;
	virtual void ClearUI() override;


	void UseSpawnedItem(TSubclassOf<class ACItem> InUseItem ,FItemDataTableBase FItem);
	// Trader
public:
	void InitFItemDataTable();
	TArray<FItemDataTableBase> GetFItemDatas() { return InvenFItems; }

	// Trader의 정보를 Player로부터 받음 ( Trader Delegate를 통해 )
	void OpenTraderWindow(const TArray<FItemDataTableBase>& InTraderFItems, const int32& InMoney);
	void CloseTraderWindow();

public:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(EditDefaultsOnly)
		UDataTable* ItemDT;

private:
	TArray<FItemDataTableBase> InvenFItems;
	TMap<TSubclassOf<class ACItem>, class ACItem*> SpawnItems;
	int32 money = 0;
};
