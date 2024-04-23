#include "Notifies/Action/CAnimNotifyState_CalculateSlash.h"

#include "Global.h"

#include "Components/CSkillComponent.h"

FString UCAnimNotifyState_CalculateSlash::GetNotifyName_Implementation() const
{
	return "CaculateSlash";
}

void UCAnimNotifyState_CalculateSlash::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	NULL_RETURN(MeshComp);
	NULL_RETURN(MeshComp->GetOwner());

	//UCSkillComponent* SkillComp = Cast<UCSkillComponent>(MeshComp->GetOwner()->GetComponentByClass(UCSkillComponent::StaticClass()));
	UCSkillComponent* SkillComp = CHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());

	NULL_RETURN(SkillComp);

	SkillComp->Begin_FromSlash();
}

void UCAnimNotifyState_CalculateSlash::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	NULL_RETURN(MeshComp);
	NULL_RETURN(MeshComp->GetOwner());

	//UCSkillComponent* SkillComp = Cast<UCSkillComponent>(MeshComp->GetOwner()->GetComponentByClass(UCSkillComponent::StaticClass()));
	UCSkillComponent* SkillComp = CHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());

	NULL_RETURN(SkillComp);

	SkillComp->End_FromSlash();
}