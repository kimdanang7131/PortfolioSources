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

    // #1. ���� �̹����� �ִ� InvenSlot�� ��ȸ�ϸ鼭 �ִ�(99)���� ������ ���� �̹����� �߰�
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

        // #2. �̹����� ���ص� �ִ�( 99 ) �� ���� ���ϸ� ��ȸ ����
        InvenSlotWidgetMap[key]->SetItemQuantity(itemNum + InNum);
        InNum = 0;
        return InNum;
    }

    return InNum;
}

void UCUserWidget_InvenWindow::AddFItemToInvenSlot(FItemDataTableBase InFItemData , bool bIsFItemQuantity , const int32& InQuantity)
{
    int32 num = 0;

    // #1. ���� ��� or Ư�� ���� �ŷ��� ���ؼ� 
    num = (bIsFItemQuantity == true) ? InFItemData.Quantity : InQuantity;

    int32 addSlotIdx = FindNearestInvenSlot();

    // #2. �κ��丮�� �� ���ְų� �����߻��� 
    if (addSlotIdx < 0)
    {
        // ������������ ���� �̹����� ���� ���
        if (InvenSlotIndexMap.Contains(InFItemData.ItemTexture))
            num = AddSameItemQuantity(num,InFItemData);

        num = 0; // �������� ���� ���� ����
        CLog::Print("AddFItemToInvenSlot Full or Fail");
        return;
    }

    // #3. �κ��丮�� ���� �������� ���� ���
    if (InvenSlotIndexMap.Contains(InFItemData.ItemTexture))
    {
        // #4. ���� �������� ������ �ܰ� ó�� -> ��� ������ �߰��� #5����
        num = AddSameItemQuantity(num, InFItemData);

        // #5. num�� �����ִ� ���¿��� ��ȸ�� ����Ǿ����� 
        // ex) �־�� �Ǵ°��� 40�� �϶�  -> 98,96 -> 99, 99 , [ 36 �߰� ]   �� ���� �� ���� �տ��ִ°��� �߰�
        if (num > 0)
        {
            InvenSlotIndexMap[InFItemData.ItemTexture].Add(addSlotIdx);
            InvenSlotWidgetMap[addSlotIdx]->SetSlotData(InFItemData , num);
            num = 0;
        }
    }
    //#6. ����� �ְ� ���ο� �̹��� �߰��ؾ��� ��
    else
    {
        TArray<int32> NewSlotIndex;
        NewSlotIndex.Add(addSlotIdx);

        InvenSlotIndexMap.Add(InFItemData.ItemTexture , NewSlotIndex);
        InvenSlotWidgetMap[addSlotIdx]->SetSlotData(InFItemData, num);
    }
}

// �ϴ� ����� �ִ� ��츸
void UCUserWidget_InvenWindow::AddFItemSelectedInvenSlot(FItemDataTableBase InFItemData , const int32& InSelectedIndex , UCUserWidget_Description* InDescWidget)
{
    UCUserWidget_InvenSlot* SelectedWidget = InvenSlotWidgetMap[InSelectedIndex];
    SelectedWidget->SwapData(InFItemData, InDescWidget);

    InvenSlotIndexMap[SelectedWidget->GetItemTexture()].Add(SelectedWidget->GetSlotIndex());
}


int32 UCUserWidget_InvenWindow::FindNearestInvenSlot()
{
    // #1. ���� ó��
    //if (InvenSlotWidgetMap.Num() != InvenMaxCount)
    //{
    //    CLog::Print("Debug : UCUserWidget_InvenWindow FindNearestInvenSlot error");
    //    return -100;
    //}

    // #2. �� ���� ��ȸ�ϴ� ���� return -> ItemTexture�� �ִ��� ������ �˻�
    for (int32 i = 0; i < InvenMaxCount; i++)
    {
        if (InvenSlotWidgetMap[i]->CheckSlotEmpty())
            return i;
    }


    // #3. ��ã���� -1 ����
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

    // Slot���� Data �ٲٱ�
    {
        // #1. Swapped�� FItem ����
        FItemDataTableBase tempFItem = swappedFItem;
        class UCUserWidget_Description* tempWidget = (*SwappedWidget)->ItemDescWidget;
        // #2. Swapped�� Swap�� FItem �ֱ�
        (*SwappedWidget)->SwapData(swapFItem, (*SwapWidget)->ItemDescWidget);

        // #3. Swap�� ������ Swapped�� Fitem �ֱ�
        (*SwapWidget)->SwapData(tempFItem , tempWidget);
    }

    // �׸� �׷����� �� -> swapTexture�� swappedIndex�� �����Ƿ�
    InvenSlotIndexMap[swapTexture].Add(InSwappedIndex);
    InvenSlotIndexMap[swappedTexture].Add(InSwapIndex);
}

void UCUserWidget_InvenWindow::ClearInvenSlot(const int32& InClearIndex)
{
    // InvenSlotWidgetMap���� InClearIndex�� �ش��ϴ� UTexture2D ������ ��������
    UTexture2D* itemKey = nullptr;

    if (InvenSlotWidgetMap.Contains(InClearIndex))
        itemKey = InvenSlotWidgetMap[InClearIndex]->GetItemTexture();

    // UTexture2D�� ��ȿ�� ���
    if (itemKey != nullptr)
    {
        // �ش� UTexture2D�� ��Ī�Ǵ� TArray<int32> ã��
        TArray<int32>* ValueArray = InvenSlotIndexMap.Find(itemKey);

        // TArray<int32>�� �����ϴ� ���
        if (ValueArray != nullptr)
        {
            // InClearIndex�� ���� ���� �ִ��� Ȯ�� �� ó��
            for (int32& Index : *ValueArray)
            {
                if (Index == InClearIndex)
                {
                    // ���� ���, IndexArray���� �ش� �� ����
                    ValueArray->Remove(InClearIndex);
                    break;
                }
            }
        }
    }

    InvenSlotWidgetMap[InClearIndex]->ClearSlot();
    //InvenSlotWidgetMap[InClearIndex] = nullptr; -> nullptr������ 3�ð� ����
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

    // #1. �巡�� �� ��쿡�� �ѹ��� ���� / �Ǹ�
    price = (bIsDrag == true) ? Seller->GetSlotWidget(InSellerIndex)->GetItemQuantity() * Seller->GetSlotWidget(InSellerIndex)->GetFItem().Price : Seller->GetSlotWidget(InSellerIndex)->GetFItem().Price;

    // #2. �����ڰ� ���Ű����� ��� / �Ұ����� ���
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

