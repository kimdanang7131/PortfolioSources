#include "Notifies/CAnimNotify_Finish.h"
#include "Global.h"
////////////////////////////////
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CWeaponStateComponent.h"
////////////////////////////////
#include "Characters/CCharacter.h"
////////////////////////////////

FString UCAnimNotify_Finish::GetNotifyName_Implementation() const
{
	return "Finish";
}

void UCAnimNotify_Finish::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	NULL_RETURN(MeshComp);
	NULL_RETURN(MeshComp->GetOwner());

	UCStateComponent*  State  = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	UCStatusComponent* Status = CHelpers::GetComponent<UCStatusComponent>(MeshComp->GetOwner());
	

	NULL_RETURN(State);

	if (State->IsDeadMode())
	{
		MeshComp->GetOwner()->Destroy();
		return;
	}

	State->SetIdleMode();
	Status->SetMove();
	Status->SetControl();
	Status->SetMovementSpeed(600);

	//Weapon¿« ResetAction«ÿµµ µ 
}
