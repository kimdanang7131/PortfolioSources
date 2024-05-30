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
    // #1. InvenSlot 값 설정들 대부분 함수들 ImplementableEvent로 블루프린트에서 직접 설정한 것들
    SetFItemData(InFItemData);
    SetItemTexture(InFItemData.ItemTexture);
    SetItemQuantity(InQuantity);

    // #2. 블프에서 직접 Class 설정 후 이 함수를 호출했을때 Widget 생성
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
    SetItemQuantity(0); // 자동 opactiy

    if (!!ItemDescWidget)
        SetDescription(nullptr); // 오류 생길 가능성

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

    // SetItemQuantity에서 Quantity가 0이되면 SetItemTexture -> nullptr들어가서 false나옴
    if (CheckSlotEmpty() == true)
    {
        ACPlayerController::GetPlayerMainUI()->GetInvenWindow()->ClearInvenSlot(Index);
    }
}