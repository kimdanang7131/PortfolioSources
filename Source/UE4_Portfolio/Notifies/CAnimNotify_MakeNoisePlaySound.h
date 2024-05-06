#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_MakeNoisePlaySound.generated.h"

UCLASS()
class UE4_PORTFOLIO_API UCAnimNotify_MakeNoisePlaySound : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;


public:
	UPROPERTY(EditAnywhere, Category = "MakeNoise")
		float Loudness = 1.f;

	UPROPERTY(EditAnywhere, Category = "SoundCue")
		class USoundCue* Sound;
};
