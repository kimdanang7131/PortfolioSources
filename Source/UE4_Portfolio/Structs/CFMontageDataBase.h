#pragma once

#include "CoreMinimal.h"

#include "CFMontageDataBase.generated.h"

USTRUCT(BlueprintType)
struct FMontageData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		bool bCanControl = true;
};

USTRUCT(BlueprintType)
struct FActionData : public FMontageData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Power = 1.0f;

	UPROPERTY(EditAnywhere)
		float HitStop = 0.f;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Trail;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShakeBase> ShakeClass;
};

USTRUCT(BlueprintType)
struct FSkillData : public FMontageData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACDoSkill> SkillClass;

	UPROPERTY(EditAnywhere)
		float Power = 1.0f;

	UPROPERTY(EditAnywhere)
		float HitStop = 0.f;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Trail;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShakeBase> ShakeClass;
};