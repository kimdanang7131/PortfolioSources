#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_EndSkill.generated.h"

UCLASS()
class UE4_PORTFOLIO_API UCAnimNotify_EndSkill : public UAnimNotify
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
