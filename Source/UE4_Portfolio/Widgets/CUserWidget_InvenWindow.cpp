#include "Widgets/CUserWidget_InvenWindow.h"

#include "Global.h"

#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Description.h"
#include "Widgets/CUserWidget_InvenSlot.h"

#include "CPlayerController.h"
#include "Widgets/CUserWidget_MainUI.h"

void UCUserWidget_InvenWindow::NativeConstruct()
{
    Super::NativeConstruct();
}

void UCUserWidget_InvenWindow::SetInvenSlotWidget(const int32& InSlotIndex, class UCUserWidget_InvenSlot* InInvenSlotWidget)
{
    if ((InSlotIndex >= 0 && InSlotIndex < InvenMaxCount + 4) && InInvenSlotWidget != nullptr)
    {
          InvenSlotWidgetMap.Add(InSlotIndex, InInvenSlotWidget);
          InInvenSlotWidget->SetConstructEvent(InSlotIndex);
    }
}

int32 UCUserWidget_InvenWindow::AddSameItemQuantity(int32 InNum , FItemDataTableBase InFItemData)
{
    TArray<int32>& SlotValues = InvenSlotIndexMap[InFItemData.ItemTexture];

    // #1. 같은 이미지가 있는 InvenSlot을 순회하면서 최대(99)개를 넘으면 다음 이미지에 추가
    for (int32 i = 0; i < SlotValues.Num(); i++)
    {
        int key = SlotValues[i];
        int itemNum = InvenSlotWidgetMap[key]->GetItemQuantity();

        if (itemNum + InNum > slotMaxCount)
        {
            InNum = itemNum + InNum - slotMaxCount;
            InvenSlotWidgetMap[key]->SetItemQuantity(slotMaxCount);
            continue;
        }

        // #2. 이미지에 더해도 최대( 99 ) 를 넘지 못하면 순회 종료
        InvenSlotWidgetMap[key]->SetItemQuantity(itemNum + InNum);
        InNum = 0;
        return InNum;
    }

    return InNum;
}

void UCUserWidget_InvenWindow::AddFItemToInvenSlot(FItemDataTableBase InFItemData , bool bIsFItemQuantity , const int32& InQuantity)
{
    int32 num = 0;

    // #1. 몬스터 드랍 or 특정 개수 거래를 위해서 
    num = (bIsFItemQuantity == true) ? InFItemData.Quantity : InQuantity;

    int32 addSlotIdx = FindNearestInvenSlot();

    // #2. 인벤토리가 꽉 차있거나 에러발생시 
    if (addSlotIdx < 0)
    {
        // 공간없을때도 같은 이미지가 들어가는 경우
        if (InvenSlotIndexMap.Contains(InFItemData.ItemTexture))
            num = AddSameItemQuantity(num,InFItemData);

        num = 0; // 꽉찬순간 남은 개수 버림
        CLog::Print("AddFItemToInvenSlot Full or Fail");
        return;
    }

    // #3. 인벤토리에 같은 아이템이 있을 경우
    if (InvenSlotIndexMap.Contains(InFItemData.ItemTexture))
    {
        // #4. 같은 아이템의 개수에 잔고 처리 -> 재고 남으면 추가는 #5에서
        num = AddSameItemQuantity(num, InFItemData);

        // #5. num이 남아있는 상태에서 순회가 종료되었을때 
        // ex) 넣어야 되는값이 40개 일때  -> 98,96 -> 99, 99 , [ 36 추가 ]   빈 공간 중 가장 앞에있는곳에 추가
        if (num > 0)
        {
            InvenSlotIndexMap[InFItemData.ItemTexture].Add(addSlotIdx);
            InvenSlotWidgetMap[addSlotIdx]->SetSlotData(InFItemData , num);
            num = 0;
        }
    }
    //#6. 빈공간 있고 새로운 이미지 추가해야할 때
    else
    {
        TArray<int32> NewSlotIndex;
        NewSlotIndex.Add(addSlotIdx);

        InvenSlotIndexMap.Add(InFItemData.ItemTexture , NewSlotIndex);
        InvenSlotWidgetMap[addSlotIdx]->SetSlotData(InFItemData, num);
    }
}

// 일단 빈곳에 넣는 경우만
void UCUserWidget_InvenWindow::AddFItemSelectedInvenSlot(FItemDataTableBase InFItemData , const int32& InSelectedIndex , UCUserWidget_Description* InDescWidget)
{
    UCUserWidget_InvenSlot* SelectedWidget = InvenSlotWidgetMap[InSelectedIndex];
    SelectedWidget->SwapData(InFItemData, InDescWidget);

    InvenSlotIndexMap[SelectedWidget->GetItemTexture()].Add(SelectedWidget->GetSlotIndex());
}


int32 UCUserWidget_InvenWindow::FindNearestInvenSlot()
{
    // #1. 예외 처리
    //if (InvenSlotWidgetMap.Num() != InvenMaxCount)
    //{
    //    CLog::Print("Debug : UCUserWidget_InvenWindow FindNearestInvenSlot error");
    //    return -100;
    //}

    // #2. 빈 곳을 순회하는 순간 return -> ItemTexture이 있는지 없는지 검사
    for (int32 i = 0; i < InvenMaxCount; i++)
    {
        if (InvenSlotWidgetMap[i]->CheckSlotEmpty())
            return i;
    }


    // #3. 못찾으면 -1 리턴
    return -1;
}


void UCUserWidget_InvenWindow::SwapInvenSlot(const int32& InSwapIndex, const int32& InSwappedIndex)
{
    UCUserWidget_InvenSlot** SwapWidget    = InvenSlotWidgetMap.Find(InSwapIndex);
    UCUserWidget_InvenSlot** SwappedWidget = InvenSlotWidgetMap.Find(InSwappedIndex);

    FItemDataTableBase swapFItem    = (*SwapWidget)->GetFItem();
    FItemDataTableBase swappedFItem = (*SwappedWidget)->GetFItem();

    ///
    UTexture2D* swapTexture    = (*SwapWidget)->GetItemTexture();
    UTexture2D* swappedTexture = (*SwappedWidget)->GetItemTexture();

    TArray<int32>& SwappedSlotValues = InvenSlotIndexMap[swappedTexture];
    for (int32 i = 0; i < SwappedSlotValues.Num(); i++)
    {
        if (SwappedSlotValues[i] == InSwappedIndex)
        {
            SwappedSlotValues.Remove(i);
            break;
        }
    }

    TArray<int32>& SwapSlotValues = InvenSlotIndexMap[swapTexture];
    for (int32 i = 0; i < SwapSlotValues.Num(); i++)
    {
        if (SwapSlotValues[i] == InSwapIndex)
        {
            SwapSlotValues.Remove(i);
            break;
        }
    }

    // Slot들의 Data 바꾸기
    {
        // #1. Swapped의 FItem 저장
        FItemDataTableBase tempFItem = swappedFItem;
        class UCUserWidget_Description* tempWidget = (*SwappedWidget)->ItemDescWidget;
        // #2. Swapped에 Swap의 FItem 넣기
        (*SwappedWidget)->SwapData(swapFItem, (*SwapWidget)->ItemDescWidget);

        // #3. Swap에 이전의 Swapped의 Fitem 넣기
        (*SwapWidget)->SwapData(tempFItem , tempWidget);
    }

    // 그림 그려보면 됨 -> swapTexture가 swappedIndex에 들어갔으므로
    InvenSlotIndexMap[swapTexture].Add(InSwappedIndex);
    InvenSlotIndexMap[swappedTexture].Add(InSwapIndex);
}

void UCUserWidget_InvenWindow::ClearInvenSlot(const int32& InClearIndex)
{
    // InvenSlotWidgetMap에서 InClearIndex에 해당하는 UTexture2D 포인터 가져오기
    UTexture2D* itemKey = nullptr;

    if (InvenSlotWidgetMap.Contains(InClearIndex))
        itemKey = InvenSlotWidgetMap[InClearIndex]->GetItemTexture();

    // UTexture2D가 유효한 경우
    if (itemKey != nullptr)
    {
        // 해당 UTexture2D에 매칭되는 TArray<int32> 찾기
        TArray<int32>* ValueArray = InvenSlotIndexMap.Find(itemKey);

        // TArray<int32>가 존재하는 경우
        if (ValueArray != nullptr)
        {
            // InClearIndex와 같은 값이 있는지 확인 후 처리
            for (int32& Index : *ValueArray)
            {
                if (Index == InClearIndex)
                {
                    // 예를 들어, IndexArray에서 해당 값 제거
                    ValueArray->Remove(InClearIndex);
                    break;
                }
            }
        }
    }

    InvenSlotWidgetMap[InClearIndex]->ClearSlot();
    //InvenSlotWidgetMap[InClearIndex] = nullptr; -> nullptr쓰지마 3시간 날림
}


void UCUserWidget_InvenWindow::ClearInventoryWindow()
{

    for (int32 i = 0; i < InvenMaxCount; i++)
        InvenSlotWidgetMap[i]->ClearSlot();

    InvenSlotIndexMap.Empty();
}


void UCUserWidget_InvenWindow::SetPlayerInvenBool()
{
    for (int32 i = 0; i < InvenMaxCount; i++)
        InvenSlotWidgetMap[i]->bIsPlayerInven = true;
}

void UCUserWidget_InvenWindow::TestPrint()
{
    for (int32 i = 0; i < InvenSlotWidgetMap.Num(); i++)
        CLog::Print(InvenSlotWidgetMap[i]->GetName());
}


TArray<FItemDataTableBase> UCUserWidget_InvenWindow::GetInvenWindowItems()
{
    TArray<FItemDataTableBase> returnItems;

    for (int32 i = 0; i < InvenSlotWidgetMap.Num(); i++)
    {
        returnItems.Add(InvenSlotWidgetMap[i]->GetFItem());
        //CLog::Print(InvenSlotWidgetMap[i]->GetItemTexture()->GetName());
    }

    return returnItems;
}



void UCUserWidget_InvenWindow::SetBuyerAndSellerWindow(UCUserWidget_InvenWindow** Buyer, UCUserWidget_InvenWindow** Seller, bool bIsFromPlayerInven)
{
    UCUserWidget_MainUI* MainUI = ACPlayerController::GetPlayerMainUI();

    if (bIsFromPlayerInven)
    {
        *Buyer  = MainUI->GetTraderWindow();
        *Seller = MainUI->GetInvenWindow();
    }
    else
    {
        *Buyer  = MainUI->GetInvenWindow();
        *Seller = MainUI->GetTraderWindow();
    }
}

bool UCUserWidget_InvenWindow::TradeItem(const int32& InSellerIndex, bool bIsDrag, bool bIsFromPlayerInven)
{
    UCUserWidget_InvenWindow* Seller = nullptr;
    UCUserWidget_InvenWindow* Buyer = nullptr;

    SetBuyerAndSellerWindow(&Buyer, &Seller, bIsFromPlayerInven);


    int32 price = 0;

    // #1. 드래그 한 경우에는 한번에 구매 / 판매
    price = (bIsDrag == true) ? Seller->GetSlotWidget(InSellerIndex)->GetItemQuantity() * Seller->GetSlotWidget(InSellerIndex)->GetFItem().Price : Seller->GetSlotWidget(InSellerIndex)->GetFItem().Price;

    // #2. 구매자가 구매가능한 경우 / 불가능한 경우
    if (Buyer->SubMoney(price) == false)
        return false;


    Seller->AddMoney(price);
    Buyer->AddFItemToInvenSlot(Seller->GetSlotWidget(InSellerIndex)->GetFItem(), bIsDrag);

    return true;
}


void UCUserWidget_InvenWindow::SetMoney(const int32& InMoney)
{
    if (InMoney < 0)
    {
        CLog::Print("Debug : SetMoney Error!");
        return;
    }

    money = InMoney;
    SetMoneyToText(money);
}

void UCUserWidget_InvenWindow::AddMoney(const int32& InMoney)
{
    money += InMoney;
    SetMoney(money);
}

bool UCUserWidget_InvenWindow::SubMoney(const int32& InMoney)
{
    if (CheckCanBuy(InMoney) == false)
        return false;
    
    money -= InMoney;
    SetMoney(money);

    return true;
}

bool UCUserWidget_InvenWindow::CheckCanBuy(const int32& InMoney)
{
    return (money - InMoney) >= 0;
}

