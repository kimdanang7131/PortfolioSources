#include "Notifies/Action/CAnimNotifyState_OnAttackTimer.h"
#include "Global.h"
/////////////////////////////////
#include "Components/UIComponents/CSkillComponent.h"


FString UCAnimNotifyState_OnAttackTimer::GetNotifyName_Implementation() const
{
	return "AttackByTimer";
}

void UCAnimNotifyState_OnAttackTimer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	NULL_RETURN(MeshComp);
	NULL_RETURN(MeshComp->GetOwner());

	//UCSkillComponent* SkillComp = Cast<UCSkillComponent>(MeshComp->GetOwner()->GetComponentByClass(UCSkillComponent::StaticClass()));
	UCSkillComponent* SkillComp = CHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());

	NULL_RETURN(SkillComp);

	SkillComp->OnAttackByTimer();
}

void UCAnimNotifyState_OnAttackTimer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	NULL_RETURN(MeshComp);
	NULL_RETURN(MeshComp->GetOwner());

	//UCSkillComponent* SkillComp = Cast<UCSkillComponent>(MeshComp->GetOwner()->GetComponentByClass(UCSkillComponent::StaticClass()));
	UCSkillComponent* SkillComp = CHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());

	NULL_RETURN(SkillComp);

	SkillComp->OffAttackByTimer();
}