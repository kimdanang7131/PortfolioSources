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
    // #1. SkillSlot 값 설정들 대부분 함수들 ImplementableEvent로 블루프린트에서 직접 설정한 것들
    Index = InIndex;
    SetFSkillData(InFSkillData);
    SetSkillTexture(InFSkillData.SkillTexture);

    // #2. 블프에서 직접 Class 설정 후 이 함수를 호출했을때 Widget 생성
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



