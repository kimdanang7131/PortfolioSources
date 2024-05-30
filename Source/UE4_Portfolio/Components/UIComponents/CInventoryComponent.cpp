#include "Components/UIComponents/CInventoryComponent.h"
#include "Global.h"
///////////////////////////////
#include "Components/CStateComponent.h"
///////////////////////////////
#include "Managers/UIManager.h"
#include "CPlayerController.h"
#include "Characters/CPlayer.h"
#include "Actors/CItem.h"
#include "Widgets/CUserWidget_MainUI.h"
#include "Widgets/CUserWidget_InvenWindow.h"
#include "Widgets/CUserWidget_InvenSlot.h"
///////////////////////////////
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"

UCInventoryComponent::UCInventoryComponent()
{
}

/** ���۽� ItemDataTable�� ��� ��鿡 ���� Data�� UI�� ��� */
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
            // #1. bCanActivate�� True�� �Ǿ��ִ� �͵鸸 Item SpawnActor
            if (arr[i]->bCanActivate == true)
            { 
                // #2. Map�� ItemClass(Value)�� ���ٸ� Spawn�ؼ� ���
                if (SpawnItems.Contains(arr[i]->ItemClass) == false)
                {
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

/** money,InvenFItems�� �÷��̾ �޾Ƽ� Trader���� �Ѱ��� */
void UCInventoryComponent::UpdateTraderInvenDatas(TArray<FItemDataTableBase> InInvenFItems, const int32& InMoney)
{
    InvenFItems.Empty();

    money       = InMoney;
    InvenFItems = InInvenFItems;
}

/** Player�� �ٷ� InvenUI ������Ʈ */
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
    }
}

void UCInventoryComponent::InitFItemDataTable()
{
    TArray<FName> ItemRowNames = ItemDT->GetRowNames();
    // #1. ������ ���̺��� �� �� �̸��� ��ȸ
    for (const FName& RowName : ItemRowNames)
    {
        // #2. �� �̸��� ����Ͽ� ������ ���̺��� �ش� �� ��������
        FItemDataTableBase* ItemData = ItemDT->FindRow<FItemDataTableBase>(RowName, TEXT("FItemDataTableBaseByInvenComp"));
        if (!!ItemData)
        {
            // #3. ������ ���� ��ȿ�� ���, ItemDatas�� �߰�
            InvenFItems.Add(*ItemData);
        }
    }
}

// Trader ������ ������ Delegate�� ���� Player���� Trader Window�� Open��Ŵ
void UCInventoryComponent::OpenTraderWindow(const TArray<FItemDataTableBase>& InTraderFItems, const int32& InMoney)
{
    UIManager::SetGameModeOnly();

    // #1. Trader���� ���� �κ��丮�� Item UI �߰�
    for (const FItemDataTableBase& FItem : InTraderFItems)
        MainUI->GetTraderWindow()->AddFItemToInvenSlot(FItem, true);

    MainUI->GetTraderWindow()->SetMoney(InMoney);
    

    // #2. ������ ����
    MainUI->SetTraderInventoryVisibility(true);
    MainUI->SetCanTrade(true);

    OpenUI();
}

/** Trader ������ ����� ��� UI�� ���� , GameMode�� ���� , Trader Window�� Clear 
    Player���� ��������Ʈ�� ���� UpdateTraderInvenDatas Trader���� ���� */
void UCInventoryComponent::CloseTraderWindow()
{
    UIManager::SetGameModeOnly();

    MainUI->SetTraderInventoryVisibility(false);
    // #1. �ٸ� Trader�� �ŷ��� ���� Trader Window�� ���
    MainUI->GetTraderWindow()->ClearInventoryWindow();
    // #2. MainUI�� ��������� Trader��ȯ ���°� �ƴϸ� �׳� ������ ���
    MainUI->SetCanTrade(false);
}


void UCInventoryComponent::TestPrint()
{
    //MainUI->GetInvenWindow()->TestPrint();
}

/** ������ ��� */
void UCInventoryComponent::UseSpawnedItem(TSubclassOf<class ACItem> InUseItem , FItemDataTableBase FItem)
{
    if (SpawnItems.Contains(InUseItem))
    {
        SpawnItems[InUseItem]->AcivateItem(FItem);
    }
}

void UCInventoryComponent::UseInvenSlotItem(const int32& InSlotInvenIndex)
{
    ACPlayerController::GetPlayerMainUI()->FindIndexInvenMainSlot(InSlotInvenIndex)->UseSlotItem();
}



