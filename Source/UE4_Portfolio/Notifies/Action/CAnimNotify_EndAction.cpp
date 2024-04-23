#include "Notifies/Action/CAnimNotify_EndAction.h"
#include "Global.h"

#include "Components/CWeaponStateComponent.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "EndAction";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	NULL_RETURN(MeshComp);
	NULL_RETURN(MeshComp->GetOwner());

	UCWeaponStateComponent* weaponState = CHelpers::GetComponent<UCWeaponStateComponent>(MeshComp->GetOwner());

	NULL_RETURN(weaponState);

	weaponState->End_DoAction();
}


