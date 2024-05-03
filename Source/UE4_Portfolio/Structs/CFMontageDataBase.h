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
		TSubclassOf<class UCameraShakeBase> ShakeClass;

	UPROPERTY(EditAnywhere)
		float Power = 1.0f;

	UPROPERTY(EditAnywhere)
		float StaminaUsage = 1.0f;

	UPROPERTY(EditAnywhere)
		float LaunchAmount = 40.0f;

	UPROPERTY(EditAnywhere)
		float HitStop = 0.f;

	UPROPERTY(EditAnywhere)
		bool bKnockDown = false;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Trail;

	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* Effect;

	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;
};

USTRUCT(BlueprintType)
struct FSkillData : public FMontageData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShakeBase> ShakeClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACDoSkill> SkillClass;

	UPROPERTY(EditAnywhere)
		float Power = 1.0f;

	UPROPERTY(EditAnywhere)
		float StaminaUsage = 1.f;

	UPROPERTY(EditAnywhere)
		float LaunchAmount = 40.0f;

	UPROPERTY(EditAnywhere)
		float HitStop = 0.f;

	UPROPERTY(EditAnywhere)
		bool bKnockDown = false;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Trail;

	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* Effect;

	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;
};