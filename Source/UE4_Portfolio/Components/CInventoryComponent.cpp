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

        // ���� ã�Ҵ��� Ȯ���մϴ�.
        if (FItem)
        {
            //CLog::Print(FItem->ItemTexture->GetName());
            (*FItem).Quantity = 45;
            MainUI->GetInvenWindow()->AddFItemToInvenSlot(*FItem, true);
        }
        else
        {
            // ���� ã�� ������ ��쿡 ���� ó��
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

        // ���� ã�Ҵ��� Ȯ���մϴ�.
        if (FItem)
        {
            //CLog::Print(FItem->ItemTexture->GetName());
            (*FItem).Quantity = 30;
            MainUI->GetInvenWindow()->AddFItemToInvenSlot(*FItem, true);
        }
        else
        {
            // ���� ã�� ������ ��쿡 ���� ó��
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
    // #1. ��ųâ�� ���� ���� ��
    if (bIsVisible)
    {
        ClearUI();

        // #2. �ٸ� UI�� ��� ������������ ���� ���� ����
        if (UIManager::CheckUIMode() == false)
            UIManager::SetGameModeOnly();
    }
    else
        OpenUI();
}

// #1. UIManger�κ��� �ٸ� UI�� �Ѱ��� �����ִ��� Ȯ���ϰ� �ٲٱ�
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
    // ������ ���̺��� �� �� �̸��� ��ȸ
    for (const FName& RowName : ItemRowNames)
    {
        // �� �̸��� ����Ͽ� ������ ���̺��� �ش� �� ��������
        FItemDataTableBase* ItemData = ItemDT->FindRow<FItemDataTableBase>(RowName, TEXT("FItemDataTableBaseByInvenComp"));
        if (!!ItemData)
        {
            // ������ ���� ��ȿ�� ���, ItemDatas�� �߰�
            InvenFItems.Add(*ItemData);
        }
    }
}

// Trader ������ ������ Delegate�� ���� Player���� Trader Window�� Open��Ŵ
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

// Trader ������ ����� ��� â�� �ݰ� Trader�� �κ��丮�� UI�κ��� �޾ƿ�
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



