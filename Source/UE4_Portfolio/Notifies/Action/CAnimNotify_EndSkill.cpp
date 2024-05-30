#include "Notifies/Action/CAnimNotify_EndSkill.h"
#include "Global.h"

#include "Components/UIComponents/CSkillComponent.h"

FString UCAnimNotify_EndSkill::GetNotifyName_Implementation() const
{
	return "EndSkill";
}

void UCAnimNotify_EndSkill::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	NULL_RETURN(MeshComp);
	NULL_RETURN(MeshComp->GetOwner());

	//UCSkillComponent* SkillComp = Cast<UCSkillComponent>(MeshComp->GetOwner()->GetComponentByClass(UCSkillComponent::StaticClass()));
	UCSkillComponent* SkillComp = CHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());

	NULL_RETURN(SkillComp);

	SkillComp->End_DoSkill();
}
