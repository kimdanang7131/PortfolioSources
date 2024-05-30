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
	/** Weapon으로부터 Skill이 만들어질때 Weapon에서 사용할 함수 */
	FORCEINLINE void SetData(FSkillData InData) { Data = InData; }
	FORCEINLINE void SetOwnerWeapon(class ACWeapon* weapon) { OwnerWeapon = weapon; }
	FORCEINLINE void SetMainSkillWidget(class UCUserWidget_MainSkillSlot* InMainSkillSlot) { MainSkillSlot = InMainSkillSlot; }
	

#pragma region Components + OwnerCharacter
protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class ACWeapon* OwnerWeapon;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;
#pragma endregion

	/** 검기 어떤거 날릴지 클래스 정하기 */
	UPROPERTY(EditAnywhere, Category = "Slash")
		TSubclassOf<class ACActor_Sub_Slash> SlashClass;

	/** 무기 피격 시 이펙트*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effect")
		class UParticleSystem* HitEffect;

	/** 무기 피격 시 사운드*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effect")
		class USoundCue* HitSound;

public:
	virtual void DoSkill() {} // 안씀
	virtual void End_DoSkill() { Deactivate(); }
	virtual void End_Hold();

	/** 서브스킬 관련 */
	virtual void DoSubSkill() { } 
	/** 검기 관련 */
	virtual void Begin_FromSlash();
	virtual void End_FromSlash();

public:
	virtual void Activate();
	virtual void Deactivate();

	/** 0.x 초마다 지속 딜*/
	virtual void OnAttackByTimer();
	virtual void OffAttackByTimer();
protected:
	/** 0.x 초마다 Weapon으로부터 OverlappedActors 가져와서  지속딜 */
	void GetWeaponOverlappedActors();

	/** 스킬사용하면서 회전 */
	void OwnerForwardRInterp(const float& deltaTime, const float& interpSpeed);

	void test();

public:
	bool CheckCanActivateSkill();

protected:
	/**  OnAttackByTimer 핸들 */
	FTimerHandle Handle;
	float attackTimerTick = 0.125f;

	/** 공격 도중 회전 / 스킬 사용되었는지 여부 */
	float rInterpSpeed = 1.5f;
	bool bCanRotate   = false;
	bool bIsActivated = false;

	/** 검기 */
	FVector fromSlashVec;
	FVector endSlashVec;

	FSkillData Data;
	int32 index = 0;

	float coolTime  = 0.f;
	float spendTime = 0.f;
	bool bOnCoolTime = false;
	class UCUserWidget_MainSkillSlot* MainSkillSlot;
};
