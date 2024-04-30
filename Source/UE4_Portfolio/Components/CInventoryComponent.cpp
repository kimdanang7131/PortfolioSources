#include "Components/CInventoryComponent.h"
#include "Global.h"

#include "CPlayerController.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_MainUI.h"

#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"

#include "CStateComponent.h"

#include "Managers/UIManager.h"
#include "Widgets/CUserWidget_InvenWindow.h"
#include "UObject/ConstructorHelpers.h"
#include "Actors/CItem.h"

UCInventoryComponent::UCInventoryComponent()
{

}

void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

    State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);

    if (!!ItemDT)
    {
        TArray<FItemDataTableBase*> arr;
        ItemDT->GetAllRows<FItemDataTableBase>(TEXT("GetAllRows"), arr);

        for (int32 i = 0; i < arr.Num(); i++)
        {
            if (arr[i]->bCanActivate == true)
            { 
                if (SpawnItems.Contains(arr[i]->ItemClass) == false)
                {
                    //CLog::Print(arr[i]->ItemName.ToString());
                    ACItem* Item = CHelpers::MySpawnActor<ACItem>(arr[i]->ItemClass, OwnerCharacter, FTransform::Identity);

                    if (!!Item)
                    {
                        Item->SetItemActivate(false);
                        SpawnItems.Add(arr[i]->ItemClass, Item);
                    }
                }
            }
        }
    }
}

void UCInventoryComponent::AddFItemToUI()
{
    if (!!ItemDT)
    {
        FName RowName = "PotionBig";

        FItemDataTableBase* FItem = ItemDT->FindRow<FItemDataTableBase>(RowName, TEXT("OptionalContextString"));

        // 행을 찾았는지 확인합니다.
        if (FItem)
        {
            //CLog::Print(FItem->ItemTexture->GetName());
            (*FItem).Quantity = 45;
            MainUI->GetInvenWindow()->AddFItemToInvenSlot(*FItem, true);
        }
        else
        {
            // 행을 찾지 못했을 경우에 대한 처리
            UE_LOG(LogTemp, Warning, TEXT("Could not find row %s in Item DataTable!"), *RowName.ToString());
        }
    }
}

void UCInventoryComponent::AddFItemToUI2()
{
    if (!!ItemDT)
    {
        FName RowName = "JewelStone";

        FItemDataTableBase* FItem = ItemDT->FindRow<FItemDataTableBase>(RowName, TEXT("OptionalContextString"));

        // 행을 찾았는지 확인합니다.
        if (FItem)
        {
            //CLog::Print(FItem->ItemTexture->GetName());
            (*FItem).Quantity = 30;
            MainUI->GetInvenWindow()->AddFItemToInvenSlot(*FItem, true);
        }
        else
        {
            // 행을 찾지 못했을 경우에 대한 처리
            UE_LOG(LogTemp, Warning, TEXT("Could not find row %s in Item DataTable!"), *RowName.ToString());
        }
    }
}

TArray<FItemDataTableBase> UCInventoryComponent::GetTraderInvenDatas()
{
    return MainUI->GetTraderWindow()->GetInvenWindowItems();
}

int32 UCInventoryComponent::GetTraderMoney()
{
    return MainUI->GetTraderWindow()->GetInvenMoney();
}

void UCInventoryComponent::UpdateTraderInvenDatas(TArray<FItemDataTableBase> InInvenFItems, const int32& InMoney)
{
    InvenFItems.Empty();

    money       = InMoney;
    InvenFItems = InInvenFItems;
    
    //for (int32 i = 0; i < InvenFItems.Num(); i++)
    //{
    //    if (SpawnItems.Contains(InvenFItems[i].ItemClass))
    //    {
    //        //SpawnActor[i]++;
    //    }
    //    else
    //    {
    //        SpawnItem.Add(InvenFItems[i].ItemClass, 0);
    //    }
    //}
}

void UCInventoryComponent::UpdatePlayerInvenDatas()
{
    InvenFItems.Empty();

    money       = MainUI->GetInvenWindow()->GetInvenMoney();
    InvenFItems = MainUI->GetInvenWindow()->GetInvenWindowItems();
}


void UCInventoryComponent::ToggleInventoryWindow()
{
    // #1. 스킬창이 열려 있을 때
    if (bIsVisible)
    {
        ClearUI();

        // #2. 다른 UI가 모두 꺼져있을때만 게임 모드로 변경
        if (UIManager::CheckUIMode() == false)
            UIManager::SetGameModeOnly();
    }
    else
        OpenUI();
}

// #1. UIManger로부터 다른 UI가 한개라도 켜져있는지 확인하고 바꾸기
void UCInventoryComponent::OpenUI()
{
    if (UIManager::CheckUIMode() == false)
        UIManager::SetUIMode();

    bIsVisible = true;
    MainUI->SetPlayerInventoryVisibility(bIsVisible);
}

void UCInventoryComponent::ClearUI()
{
    if (bIsVisible)
    {
        bIsVisible = false;
        MainUI->SetPlayerInventoryVisibility(bIsVisible);
        //UpdatePlayerInvenDatas();

        //for (int32 i = 0; i < InvenFItems.Num(); i++)
        //{
        //    if(InvenFItems.Contains(i))
        //    CLog::Print(InvenFItems[i].ItemName.ToString());
        //}
    }
}

void UCInventoryComponent::InitFItemDataTable()
{
    TArray<FName> ItemRowNames = ItemDT->GetRowNames();
    // 데이터 테이블의 각 행 이름을 순회
    for (const FName& RowName : ItemRowNames)
    {
        // 행 이름을 사용하여 데이터 테이블에서 해당 행 가져오기
        FItemDataTableBase* ItemData = ItemDT->FindRow<FItemDataTableBase>(RowName, TEXT("FItemDataTableBaseByInvenComp"));
        if (!!ItemData)
        {
            // 가져온 행이 유효한 경우, ItemDatas에 추가
            InvenFItems.Add(*ItemData);
        }
    }
}

// Trader 영역에 들어오면 Delegate를 통해 Player에서 Trader Window를 Open시킴
void UCInventoryComponent::OpenTraderWindow(const TArray<FItemDataTableBase>& InTraderFItems, const int32& InMoney)
{
    UIManager::SetGameModeOnly();

    for (const FItemDataTableBase& FItem : InTraderFItems)
        MainUI->GetTraderWindow()->AddFItemToInvenSlot(FItem, true);

    MainUI->GetTraderWindow()->SetMoney(InMoney);
    MainUI->SetTraderInventoryVisibility(true);
    MainUI->SetCanTrade(true);

    OpenUI();
}

// Trader 영역을 벗어나면 모든 창을 닫고 Trader의 인벤토리를 UI로부터 받아옴
void UCInventoryComponent::CloseTraderWindow()
{
    UIManager::SetGameModeOnly();

    MainUI->SetTraderInventoryVisibility(false);
    MainUI->GetTraderWindow()->ClearInventoryWindow();
    MainUI->SetCanTrade(false);
}


void UCInventoryComponent::TestPrint()
{
    //MainUI->GetInvenWindow()->TestPrint();
}

void UCInventoryComponent::UseSpawnedItem(TSubclassOf<class ACItem> InUseItem , FItemDataTableBase FItem)
{
    if (SpawnItems.Contains(InUseItem))
    {
        SpawnItems[InUseItem]->AcivateItem(FItem);
    }
}



