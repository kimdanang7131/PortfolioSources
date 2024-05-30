#include "Actors/CItem_Potion.h"
#include "Global.h"
/////////////////////////
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
/////////////////////////
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"

ACItem_Potion::ACItem_Potion()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> AnimMontage(TEXT("AnimMontage'/Game/Blueprints/Animations/Common/Player/Drink_Montage.Drink_Montage'"));
	if (AnimMontage.Succeeded())
	{
		DrinkMontage = AnimMontage.Object;
	}
}

void ACItem_Potion::AcivateItem(FItemDataTableBase FItem)
{
	if (!!State)
	{
		if (State->IsIdleMode() == false)
			return;
	}

	if (!!Status)
	{
		Status->AddHealth(amount);

		/** Idle상태에서만 Drink 애니메이션 실행 */
		ACharacter* Character = Cast<ACharacter>(OwnerCharacter);
		if (Character)
		{
			if (State->IsIdleMode())
			{
				
				State->SetActionMode();
				Character->PlayAnimMontage(DrinkMontage);
			}
		}
	}
}

