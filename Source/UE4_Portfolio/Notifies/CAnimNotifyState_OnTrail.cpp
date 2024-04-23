#include "Notifies/CAnimNotifyState_OnTrail.h"

#include "Global.h"

#include "Components/CWeaponStateComponent.h"

FString UCAnimNotifyState_OnTrail::GetNotifyName_Implementation() const
{
	return "Trail";
}

void UCAnimNotifyState_OnTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	NULL_RETURN(MeshComp);
	NULL_RETURN(MeshComp->GetOwner());

	UCWeaponStateComponent* weaponState = CHelpers::GetComponent<UCWeaponStateComponent>(MeshComp->GetOwner());
	NULL_RETURN(weaponState);

	weaponState->OnTrail();
}

void UCAnimNotifyState_OnTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	NULL_RETURN(MeshComp);
	NULL_RETURN(MeshComp->GetOwner());

	UCWeaponStateComponent* weaponState = CHelpers::GetComponent<UCWeaponStateComponent>(MeshComp->GetOwner());
	NULL_RETURN(weaponState);

	weaponState->OffTrail();
}