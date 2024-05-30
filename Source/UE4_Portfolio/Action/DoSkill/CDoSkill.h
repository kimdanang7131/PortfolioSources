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
	/** Weapon���κ��� Skill�� ��������� Weapon���� ����� �Լ� */
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

	/** �˱� ��� ������ Ŭ���� ���ϱ� */
	UPROPERTY(EditAnywhere, Category = "Slash")
		TSubclassOf<class ACActor_Sub_Slash> SlashClass;

	/** ���� �ǰ� �� ����Ʈ*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effect")
		class UParticleSystem* HitEffect;

	/** ���� �ǰ� �� ����*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effect")
		class USoundCue* HitSound;

public:
	virtual void DoSkill() {} // �Ⱦ�
	virtual void End_DoSkill() { Deactivate(); }
	virtual void End_Hold();

	/** ���꽺ų ���� */
	virtual void DoSubSkill() { } 
	/** �˱� ���� */
	virtual void Begin_FromSlash();
	virtual void End_FromSlash();

public:
	virtual void Activate();
	virtual void Deactivate();

	/** 0.x �ʸ��� ���� ��*/
	virtual void OnAttackByTimer();
	virtual void OffAttackByTimer();
protected:
	/** 0.x �ʸ��� Weapon���κ��� OverlappedActors �����ͼ�  ���ӵ� */
	void GetWeaponOverlappedActors();

	/** ��ų����ϸ鼭 ȸ�� */
	void OwnerForwardRInterp(const float& deltaTime, const float& interpSpeed);

	void test();

public:
	bool CheckCanActivateSkill();

protected:
	/**  OnAttackByTimer �ڵ� */
	FTimerHandle Handle;
	float attackTimerTick = 0.125f;

	/** ���� ���� ȸ�� / ��ų ���Ǿ����� ���� */
	float rInterpSpeed = 1.5f;
	bool bCanRotate   = false;
	bool bIsActivated = false;

	/** �˱� */
	FVector fromSlashVec;
	FVector endSlashVec;

	FSkillData Data;
	int32 index = 0;

	float coolTime  = 0.f;
	float spendTime = 0.f;
	bool bOnCoolTime = false;
	class UCUserWidget_MainSkillSlot* MainSkillSlot;
};
