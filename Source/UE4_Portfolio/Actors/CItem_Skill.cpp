#include "Actors/CItem_Skill.h"
#include "Global.h"

#include "CPlayerController.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_MainUI.h"
#include "Widgets/CUserWidget_SkillWindow.h"

void ACItem_Skill::AcivateItem(FItemDataTableBase FItem)
{
	//FString str = FItem.ItemName.ToString();
	//
	//FString last = str.Right(1);
	//
	//// #1. Skill이 맞는지 확인
	//if (last.Equals(TEXT("1")) || last.Equals(TEXT("2")) || last.Equals(TEXT("3")))
	//{
	//	int num = FCString::Atoi(*last) - 1;
	//
	//	//FString SkillName =
	//	//ACPlayerController::GetPlayerMainUI()->GetSkillWindow()->
	//}
	
	UTexture2D* skillTexture = FItem.ItemTexture;

	ACPlayerController::GetPlayerMainUI()->GetSkillWindow()->UseSkillItem(skillTexture);
	ACPlayerController::GetPlayerMainUI()->SetMainSkillActivated(skillTexture);
}