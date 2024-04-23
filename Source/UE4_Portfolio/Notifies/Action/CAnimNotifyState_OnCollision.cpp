#include "Notifies/Action/CAnimNotifyState_OnCollision.h"

#include "Global.h"

#include "Components/CWeaponStateComponent.h"

FString UCAnimNotifyState_OnCollision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCAnimNotifyState_OnCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	NULL_RETURN(MeshComp);
	NULL_RETURN(MeshComp->GetOwner());

	UCWeaponStateComponent* weaponState = CHelpers::GetComponent<UCWeaponStateComponent>(MeshComp->GetOwner());
	NULL_RETURN(weaponState);

	weaponState->OnCollision();
}

void UCAnimNotifyState_OnCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	NULL_RETURN(MeshComp);
	NULL_RETURN(MeshComp->GetOwner());

	UCWeaponStateComponent* weaponState = CHelpers::GetComponent<UCWeaponStateComponent>(MeshComp->GetOwner());
	NULL_RETURN(weaponState);

	weaponState->OffCollision();
}