#include "Widgets/CUserWidget_InvenSlot.h"

#include "Global.h"

#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Description.h"

#include "Widgets/CUserWidget_MainUI.h"

#include "Widgets/CUserWidget_InvenWindow.h"
#include "Actors/CItem.h"
#include "Components/UIComponents/CInventoryComponent.h"
#include "CPlayerController.h"


void UCUserWidget_InvenSlot::NativeConstruct()
{
    Super::NativeConstruct();

}

void UCUserWidget_InvenSlot::SetConstructEvent(const int32& InIndex)
{
    Index = InIndex;
}

void UCUserWidget_InvenSlot::SetSlotData(const FItemDataTableBase& InFItemData, const int32& InQuantity)
{
    // #1. InvenSlot �� ������ ��κ� �Լ��� ImplementableEvent�� �������Ʈ���� ���� ������ �͵�
    SetFItemData(InFItemData);
    SetItemTexture(InFItemData.ItemTexture);
    SetItemQuantity(InQuantity);

    // #2. �������� ���� Class ���� �� �� �Լ��� ȣ�������� Widget ����
    if (!!ItemDescClass)
    {
        ItemDescWidget = CreateWidget<UCUserWidget_Description>(GetWorld(), ItemDescClass);

        if (!!ItemDescWidget)
        {
            SetDescriptionToCanvas(ItemDescWidget);
            SetDescription(ItemDescWidget);
        }
    }

    SetSlotOpacity();
}

void UCUserWidget_InvenSlot::SwapData(const FItemDataTableBase& InFItemData , UCUserWidget_Description* InItemDescWidget)
{
    SetFItemData(InFItemData);
    SetItemTexture(InFItemData.ItemTexture);
    SetItemQuantity(InFItemData.Quantity);
    SetDescription(InItemDescWidget);

    SetSlotOpacity();
}

bool UCUserWidget_InvenSlot::CheckSlotEmpty()
{
    return (ItemTexture == nullptr);
}

bool UCUserWidget_InvenSlot::CheckSameItem(const UTexture2D* InTexture)
{
    return (InTexture == ItemTexture);
}

void UCUserWidget_InvenSlot::ClearSlot()
{
    FItemDataTableBase FItemDataNull;

    SetFItemData(FItemDataNull);
    SetItemTexture(nullptr);
    SetItemQuantity(0); // �ڵ� opactiy

    if (!!ItemDescWidget)
        SetDescription(nullptr); // ���� ���� ���ɼ�

    SetSlotOpacity();
}

void UCUserWidget_InvenSlot::SetFItemQuantity(const int32& InQuantity)
{
    FItemData.Quantity = InQuantity;
}

void UCUserWidget_InvenSlot::UseSlotItem()
{
    if (CheckSlotEmpty() == true)
    {
        CLog::Print("UseSlotItem None");
        return;
    }

    if(FItemData.bCanActivate == false)
    {
        CLog::Print("UseSlotItem Cant use");
        return;
    }

    SetItemQuantity(FItemData.Quantity - 1);

    if (FItemData.ItemClass != nullptr)
    {
        ACPlayerController::GetPlayerMainUI()->InvenComp->UseSpawnedItem(FItemData.ItemClass , FItemData);
    }

    // SetItemQuantity���� Quantity�� 0�̵Ǹ� SetItemTexture -> nullptr���� false����
    if (CheckSlotEmpty() == true)
    {
        ACPlayerController::GetPlayerMainUI()->GetInvenWindow()->ClearInvenSlot(Index);
    }
}