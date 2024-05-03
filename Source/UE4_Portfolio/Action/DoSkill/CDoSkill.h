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

public:
	virtual void DoSkill() {} // �Ⱦ�
	virtual void End_DoSkill() { Deactivate(); }
	virtual void End_Hold();

	virtual void DoSubSkill() { } 
	// �˱�
	virtual void Begin_FromSlash();
	virtual void End_FromSlash();

public:
	virtual void Activate();
	virtual void Deactivate();

	// 0.x �ʸ��� ���ӵ�
	virtual void OnAttackByTimer();
	virtual void OffAttackByTimer();
protected:
	// 0.x �ʸ��� OverlappedActors ���ӵ�
	void GetWeaponOverlappedActors();

	// ��ų����ϸ鼭 ȸ��
	void OwnerForwardRInterp(const float& deltaTime, const float& interpSpeed);

	void test();

protected:
	// OnAttackByTimer �ڵ�
	FTimerHandle Handle;
	float attackTimerTick = 0.125f;

	// ���� ���� ȸ�� / ��ų ���Ǿ����� ����
	float rInterpSpeed = 1.5f;
	bool bCanRotate   = false;
	bool bIsActivated = false;

	// �˱�
	FVector fromSlashVec;
	FVector endSlashVec;

	FSkillData Data;
	int32 index = 0;
};
