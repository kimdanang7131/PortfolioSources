#include "Actors/CItem_Potion.h"
#include "Global.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

void ACItem_Potion::AcivateItem(FItemDataTableBase FItem)
{
	CLog::Print("ActivateItem");

	if (!!State)
	{
		if (State->IsIdleMode() == false)
			return;
	}

	if (!!Status)
	{
		Status->AddHealth(amount);
		CLog::Print(amount);
	}
}

