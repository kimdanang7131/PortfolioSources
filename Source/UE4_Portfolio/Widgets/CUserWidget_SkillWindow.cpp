#include "Widgets/CUserWidget_SkillWindow.h"
#include "Global.h"

#include "Widgets/CUserWidget_SkillSlot.h"


void UCUserWidget_SkillWindow::UseSkillItem(UTexture2D* InTexture)
{
	for (int32 i = 0; i < SkillSlotWidgets.Num(); i++)
	{
		if (SkillSlotWidgets[i]->GetSkillTexture() == InTexture)
		{
			SkillSlotWidgets[i]->SetSkillActivated(true);
			break;
		}
	}
}