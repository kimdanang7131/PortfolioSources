#include "Widgets/CUserWidget_MainUI.h"
#include "Global.h"

#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_InvenWindow.h"
#include "Widgets/CUserWidget_SkillWindow.h"

#include "Widgets/CUserWidget_MainSkillSlot.h"
#include "Characters/CPlayer.h"


void UCUserWidget_MainUI::NativeConstruct()
{
    Super::NativeConstruct();
   
    Init();

}

void UCUserWidget_MainUI::Init()
{
    if (!!SkillMainSlot_0)
    {
        SkillMainSlot_0->UseSkillIndex = 0;
        SkillMainSlots.Add(SkillMainSlot_0);
    }

    if (!!SkillMainSlot_1)
    {
        SkillMainSlot_1->UseSkillIndex = 1;
        SkillMainSlots.Add(SkillMainSlot_1);
    }

    if (!!SkillMainSlot_2)
    {
        SkillMainSlot_2->UseSkillIndex = 2;
        SkillMainSlots.Add(SkillMainSlot_2);
    }

    if (!!SkillMainSlot_3)
    {
        SkillMainSlot_3->UseSkillIndex = 3;
        SkillMainSlots.Add(SkillMainSlot_3);
    }

    OwnerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(this,0));

    if (!!OwnerCharacter)
    {
        InvenComp = CHelpers::GetComponent<UCInventoryComponent>(OwnerCharacter);
    }
}

UCUserWidget_MainSkillSlot* UCUserWidget_MainUI::FindIndexSkillMainSlot(const int32& InUseSkillIndex)
{
    // UseSkillIndex
    switch (InUseSkillIndex)
    {
    case 0 :
        return SkillMainSlot_0;
    case 1:
        return SkillMainSlot_1;
    case 2:
        return SkillMainSlot_2;
    case 3:
        return SkillMainSlot_3;
    default:
        return nullptr;
    }
}

int32 UCUserWidget_MainUI::CheckSameSkillMainSlot(UTexture2D* InTexture)
{
    // #1. 같은 Texture를 찾으면 그 Index를 반환
    for (int32 i = 0; i < SkillMainSlots.Num(); i++)
    {
        if (InTexture == SkillMainSlots[i]->SkillTexture)
            return i;
    }

    // #2. 못찾으면 -1
    return -1;
}

void UCUserWidget_MainUI::SetMainSkillActivated(UTexture2D* InTexture)
{
    // #1. 같은 Texture를 찾으면 그 Index를 반환
    for (int32 i = 0; i < SkillMainSlots.Num(); i++)
    {
        if (SkillMainSlots[i]->GetSkillTexture() == InTexture)
        {
            SkillMainSlots[i]->SetSkillActivated(true);
            break;
        }
    }
}


void UCUserWidget_MainUI::SkillSlotSwap(const int32& InSwapIndex, const int32& InSwappedIndex)
{
    UCUserWidget_MainSkillSlot* SwapMainSlot    = FindIndexSkillMainSlot(InSwapIndex);
    UCUserWidget_MainSkillSlot* SwappedMainSlot = FindIndexSkillMainSlot(InSwappedIndex);

    // #1. SwappedMainSlot의 정보저장
    UTexture2D* SkillTexture                  = SwappedMainSlot->SkillTexture;
    FSkillDataTable FSkillData                = SwappedMainSlot->FSkillData;
    UCUserWidget_Description* SkillDescWidget = SwappedMainSlot->SkillDescWidget;

    // #2. SwappedIndex에 SwapIndex 정보저장
    SwappedMainSlot->SwapData(SwapMainSlot);

    // #3. SwapIndex에 Temp 정보 저장
    SwapMainSlot->SwapDatas(SkillTexture, FSkillData, SkillDescWidget);
}

void UCUserWidget_MainUI::SkillSlotClear(class UCUserWidget_MainSkillSlot* InMainSkillSlot)
{
    InMainSkillSlot->ClearSlot();
}

TArray<TMap<EWeaponStateType, int32>> UCUserWidget_MainUI::UpdateMainSlotSkills()
{
    TArray<TMap<EWeaponStateType, int32>> SkillSlotDatas;

    for (int32 i = 0; i < SkillMainSlots.Num(); i++)
    {
        TMap<EWeaponStateType, int32> tempMap;

        if (SkillMainSlots[i]->SkillTexture == nullptr)
        {
            EWeaponStateType tempState = EWeaponStateType::Unarmed;
            int32 tempSkillIndex = -1;

            tempMap.Add(tempState, tempSkillIndex);
            SkillSlotDatas.Add(tempMap);
            continue;
        }

        EWeaponStateType tempState = SkillMainSlots[i]->FSkillData.WeaponType;
        int32 tempSkillIndex = SkillMainSlots[i]->FSkillData.SkillIndex;

        tempMap.Add(SkillMainSlots[i]->FSkillData.WeaponType, SkillMainSlots[i]->FSkillData.SkillIndex);

        SkillSlotDatas.Add(tempMap);
    }
    return SkillSlotDatas;
}



