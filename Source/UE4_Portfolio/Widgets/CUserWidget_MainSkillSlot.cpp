#include "Widgets/CUserWidget_MainSkillSlot.h"

#include "Global.h"

#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Description.h"

void UCUserWidget_MainSkillSlot::NativeConstruct()
{
    Super::NativeConstruct();
}

void UCUserWidget_MainSkillSlot::SwapData(UCUserWidget_MainSkillSlot* SwapSlot)
{
    SetFSkillData(SwapSlot->FSkillData);
    SetSkillTexture(SwapSlot->SkillTexture);
    SetDescription(SwapSlot->SkillDescWidget);
}

void UCUserWidget_MainSkillSlot::ClearSlot()
{
    FSkillDataTable NullFSkillData;
    UTexture2D* NullSkillTexture = nullptr;
    class UCUserWidget_Description* NullSkillDescWidget = nullptr;

    SetFSkillData(NullFSkillData);
    SetSkillTexture(NullSkillTexture);
    SetDescription(NullSkillDescWidget);

    SetSlotOpacity();
}

bool UCUserWidget_MainSkillSlot::CheckSlotEmpty()
{
    return (SkillTexture == nullptr);
}

void UCUserWidget_MainSkillSlot::SwapDatas(UTexture2D* InSkillTexture, FSkillDataTable InFSkillData, class UCUserWidget_Description* InSkillDescWidget)
{
    SetFSkillData(InFSkillData);
    SetSkillTexture(InSkillTexture);
    SetDescription(InSkillDescWidget);
}