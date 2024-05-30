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

/** 시작시 ItemDataTable의 모든 행들에 대한 Data를 UI에 등록 */
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
            // #1. bCanActivate이 True로 되어있는 것들만 Item SpawnActor
            if (arr[i]->bCanActivate == true)
            { 
                // #2. Map에 ItemClass(Value)가 없다면 Spawn해서 등록
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

/** money,InvenFItems를 플레이어가 받아서 Trader에게 넘겨줌 */
void UCInventoryComponent::UpdateTraderInvenDatas(TArray<FItemDataTableBase> InInvenFItems, const int32& InMoney)
{
    InvenFItems.Empty();

    money       = InMoney;
    InvenFItems = InInvenFItems;
}

/** Player는 바로 InvenUI 업데이트 */
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
    }
}

void UCInventoryComponent::InitFItemDataTable()
{
    TArray<FName> ItemRowNames = ItemDT->GetRowNames();
    // #1. 데이터 테이블의 각 행 이름을 순회
    for (const FName& RowName : ItemRowNames)
    {
        // #2. 행 이름을 사용하여 데이터 테이블에서 해당 행 가져오기
        FItemDataTableBase* ItemData = ItemDT->FindRow<FItemDataTableBase>(RowName, TEXT("FItemDataTableBaseByInvenComp"));
        if (!!ItemData)
        {
            // #3. 가져온 행이 유효한 경우, ItemDatas에 추가
            InvenFItems.Add(*ItemData);
        }
    }
}

// Trader 영역에 들어오면 Delegate를 통해 Player에서 Trader Window를 Open시킴
void UCInventoryComponent::OpenTraderWindow(const TArray<FItemDataTableBase>& InTraderFItems, const int32& InMoney)
{
    UIManager::SetGameModeOnly();

    // #1. Trader여는 순간 인벤토리에 Item UI 추가
    for (const FItemDataTableBase& FItem : InTraderFItems)
        MainUI->GetTraderWindow()->AddFItemToInvenSlot(FItem, true);

    MainUI->GetTraderWindow()->SetMoney(InMoney);
    

    // #2. 나머지 설정
    MainUI->SetTraderInventoryVisibility(true);
    MainUI->SetCanTrade(true);

    OpenUI();
}

/** Trader 영역을 벗어나면 모든 UI를 종료 , GameMode로 변경 , Trader Window를 Clear 
    Player에서 델리게이트를 통해 UpdateTraderInvenDatas Trader에게 전달 */
void UCInventoryComponent::CloseTraderWindow()
{
    UIManager::SetGameModeOnly();

    MainUI->SetTraderInventoryVisibility(false);
    // #1. 다른 Trader와 거래를 위해 Trader Window를 비움
    MainUI->GetTraderWindow()->ClearInventoryWindow();
    // #2. MainUI도 설정해줘야 Trader교환 상태가 아니면 그냥 아이템 사용
    MainUI->SetCanTrade(false);
}


void UCInventoryComponent::TestPrint()
{
    //MainUI->GetInvenWindow()->TestPrint();
}

/** 아이템 사용 */
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



