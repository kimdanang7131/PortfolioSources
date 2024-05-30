#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CWeaponStateComponent.h"
#include "Action/CActionDataAsset.h"

#include "CWeapon.generated.h"

/** DoAction�Ҷ� ������ Character���� �󸶸�ŭ Launch������ ���� ���� �Ѱ��� ��������Ʈ */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLaunchCharacter, const float& inLaunchAmount);

UCLASS()
class UE4_PORTFOLIO_API ACWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACWeapon();

protected:
	virtual void BeginPlay() override;

	/** ��Ƽĳ��Ʈ ��������Ʈ�� �̿��Ͽ� BP�� ������ ��� ShapeComponents�� OnComponentBeginOverlap -> ��� 
		��Ƽĳ��Ʈ�� ���Ͽ� �� ������ DualWeapon ���� */
public:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** DoAction�Ҷ� ������ Character���� �󸶸�ŭ Launch������ ���� ���� �Ѱ��� ��������Ʈ */
public:
	FOnLaunchCharacter OnLaunchCharacterDelegate;

public:
	/** ActionData�κ��� SkillData ���, Index�� �Ź� Skill���� ȣ��, Skill DoAction�� �����Ǿ��ִ� �κ� ��ų���� ȣ�� */
	FORCEINLINE void SetSkillDatas(TArray<FSkillData> InDatas) { SkillDatas = InDatas; }
	FORCEINLINE void SetSkillIndex(const int32& InIndex) { SkillIndex = InIndex; }
	FORCEINLINE void ExecuteSkillLaunchAmountBroadCast(const float& inLaunchAmount) { OnLaunchCharacterDelegate.Broadcast(inLaunchAmount); }
	FORCEINLINE TSet<class AActor*> GetOverlappedActors() { return OverlappedActors; }

	/** ActionData�κ��� WeaponData ��� */
	FORCEINLINE void SetDatas(TArray<FActionData> InDatas) { Datas = InDatas; }
	FORCEINLINE void SetWeaponStateType(const EWeaponStateType& type) { Type = type; }
	FORCEINLINE int32 GetWeaponStateType() { return static_cast<int32>(Type); }

	/** Notify */
	FORCEINLINE void EnableCombo()  { bCanCombo = true; }
	FORCEINLINE void DisableCombo() { bCanCombo = false; }

	/** �������Ʈ���� ����� �Լ��� */
public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnEquip();
	UFUNCTION(BlueprintImplementableEvent)
		void OnUnequip();

	UFUNCTION(BlueprintCallable)
		void AttachTo(FName InSocketName);

public:
	/** Hitstop ��� ��*/
	UFUNCTION()
		void RestoreDilation();

#pragma region SocketName , OwnerMesh
public:
	/** ���� �̸�, �������Ʈ���� ��� ���� */
	UFUNCTION(BlueprintCallable)
		USkeletalMeshComponent* GetOwnerMesh();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		 FName GetSocketOnEquippedLeft() {    return TEXT("hand_l_weapon"); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		 FName GetSocketOnEquippedRight() {   return TEXT("hand_r_weapon"); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		 FName GetSocketOnUnequippedLeft() {  return TEXT("spine_03_l_weapon"); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		 FName GetSocketOnUnequippedRight() { return TEXT("spine_03_r_weapon"); }

	FORCEINLINE FName GetParticleLocationSocketName() { return TEXT("center"); }
		
#pragma endregion

#pragma region Components + OwnerCharacter
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EWeaponStateType Type;

	// Weapon ActorComponent�� / OwnerCharacter
protected:
	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

	UPROPERTY(BlueprintReadOnly)
		class UCWeaponStateComponent* WeaponState;


	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(BlueprintReadWrite)
		class ACharacter* OwnerCharacter;


	/** ���� �ǰ� �� ����*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effect")
		class UParticleSystem* BloodEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effect")
		class USoundCue* HitSound;

#pragma endregion 

	// Notify ���� �Ϲ��Լ�
public:
	// DoAction
	void OnCollision();
	void OffCollision();

	void DoAction();
	void Begin_DoAction();

	void End_DoAction();
	void ActionSettings();

	// DoSkill
	void CreateWeaponSkills();
	class ACDoSkill* GetWeaponSkill(const int32& num);
	void SetSkillWeaponVisibility(bool bIsVisible);

	// ����
	void OnTrail();
	void OffTrail();

	// �޺� ���� �ǰݽ� Index�ʱ�ȭ �� ����
	void ResetAction();

protected:
	// DoAction
	TArray<FActionData> Datas;
	TSet<class AActor*> OverlappedActors;
	bool bCanCombo = false;
	bool bComboActivated = false;
	int32 Index = 0;

	// DoSkill
	TArray<FSkillData> SkillDatas;
	TArray<class ACDoSkill*> WeaponSkills;
	int32 SkillIndex = 0;

	/// test
	int32 n = 0;

private:
	// Property ����
	TArray<class USkeletalMeshComponent*> MeshComponents;
	TArray<class UShapeComponent*> ShapeComponents;
	TArray<class UParticleSystemComponent*> ParticleSystemComponents;
	//TSubclassOf<class UCMatineeCameraShake> camShake;
};



