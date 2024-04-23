#include "Notifies/Action/CAnimNotifyState_EnableCombo.h"

#include "Global.h"
#include "Components/CWeaponStateComponent.h"

FString UCAnimNotifyState_EnableCombo::GetNotifyName_Implementation() const
{
	return "EnableCombo";
}

void UCAnimNotifyState_EnableCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	NULL_RETURN(MeshComp);
	NULL_RETURN(MeshComp->GetOwner());

	UCWeaponStateComponent* weaponState = CHelpers::GetComponent<UCWeaponStateComponent>(MeshComp->GetOwner());
	NULL_RETURN(weaponState);

	weaponState->EnableCombo();

}

void UCAnimNotifyState_EnableCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	NULL_RETURN(MeshComp);
	NULL_RETURN(MeshComp->GetOwner());

	UCWeaponStateComponent* weaponState = CHelpers::GetComponent<UCWeaponStateComponent>(MeshComp->GetOwner());
	NULL_RETURN(weaponState);

	weaponState->DisableCombo();
}
