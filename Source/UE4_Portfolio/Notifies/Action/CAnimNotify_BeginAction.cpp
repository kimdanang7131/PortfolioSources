#include "Notifies/Action/CAnimNotify_BeginAction.h"
#include "Global.h"
#include "Characters/CCharacter.h"
#include "Components/CWeaponStateComponent.h"

FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return "BeginAction";
}


void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	NULL_RETURN(MeshComp);
	NULL_RETURN(MeshComp->GetOwner());

	UCWeaponStateComponent* weaponState = CHelpers::GetComponent<UCWeaponStateComponent>(MeshComp->GetOwner());

	NULL_RETURN(weaponState);

	weaponState->Begin_DoAction();
}
