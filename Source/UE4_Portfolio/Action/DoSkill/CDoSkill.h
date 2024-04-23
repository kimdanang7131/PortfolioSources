#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Action/CActionDataAsset.h"
#include "CDoSkill.generated.h"


UCLASS()
class UE4_PORTFOLIO_API ACDoSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDoSkill();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE void SetData(FSkillData InData) { Data = InData; }
	FORCEINLINE void SetOwnerWeapon(class ACWeapon* weapon) { OwnerWeapon = weapon; }

public:
	virtual void DoSkill() {} // ¾È¾¸
	virtual void End_DoSkill() { Deactivate(); }
	virtual void End_Hold();

	virtual void DoSubSkill() { } 
	virtual void Begin_FromSlash();
	virtual void End_FromSlash();

public:
	virtual void Activate();
	virtual void Deactivate();

	virtual void OnAttackByTimer();
	virtual void OffAttackByTimer();

	void OwnerFowardRInterp(const float& DeltaTime, const float& InterpSpeed);

	//virtual void Begin_DoSkill() {}
	void test();

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class ACWeapon* OwnerWeapon;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;


protected:
	void GetWeaponOverlappedActors();

protected:
	FTimerHandle Handle;

	FSkillData Data;
	bool bIsActivated = false;
	bool bCanRotate = false;
	float rInterpSpeed = 1.5f;

	float attackTimerTick = 0.125f;

	FVector FromSlashVec;
	FVector EndSlashVec;

	TSubclassOf<class ACActor_Sub_Slash> SlashClass;

	int32 index = 0;
};
