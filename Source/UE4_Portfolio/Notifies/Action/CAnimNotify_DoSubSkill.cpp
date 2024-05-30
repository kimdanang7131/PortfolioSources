#include "Notifies/Action/CAnimNotify_DoSubSkill.h"

#include "Global.h"
#include "Characters/CCharacter.h"

#include "Components/UIComponents/CSkillComponent.h"


FString UCAnimNotify_DoSubSkill::GetNotifyName_Implementation() const
{
	return "DoSubSkill";
}

void UCAnimNotify_DoSubSkill::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	NULL_RETURN(MeshComp);
	NULL_RETURN(MeshComp->GetOwner());

	//UCSkillComponent* Skill = Cast<UCSkillComponent>(MeshComp->GetOwner()->GetComponentByClass(UCSkillComponent::StaticClass()));
	UCSkillComponent* SkillComp = CHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());

	NULL_RETURN(SkillComp);

	SkillComp->DoSubSkill();
}