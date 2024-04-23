#include "Widgets/CUserWidget_SkillSlot.h"

#include "Global.h"

#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Description.h"

//#include "Widgets/CUserWidget_MainUI.h"


void UCUserWidget_SkillSlot::NativeConstruct()
{
    Super::NativeConstruct();

}

void UCUserWidget_SkillSlot::SetConstructEvent(int32 InIndex, FSkillDataTable InFSkillData)
{
    // #1. SkillSlot �� ������ ��κ� �Լ��� ImplementableEvent�� �������Ʈ���� ���� ������ �͵�
    Index = InIndex;
    SetFSkillData(InFSkillData);
    SetSkillTexture(InFSkillData.SkillTexture);

    // #2. �������� ���� Class ���� �� �� �Լ��� ȣ�������� Widget ����
    if (!!SkillDescClass)
    {
        SkillDescWidget = CreateWidget<UCUserWidget_Description>(GetWorld(), SkillDescClass);

        if (!!SkillDescWidget)
        {
            SetDescriptionToCanvas(SkillDescWidget);
            SetDescription(SkillDescWidget);
        }
    }
}



