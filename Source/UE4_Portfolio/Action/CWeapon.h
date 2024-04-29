#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Action/CActionDataAsset.h"
#include "GameFramework/Character.h"
#include "Components/CWeaponStateComponent.h"
#include "CWeapon.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACWeapon();

protected:
	virtual void BeginPlay() override;

	/// <ComponentBeginOverlap>
    /// ��Ƽĳ��Ʈ ��������Ʈ�� �̿��Ͽ� BP�� ������ ��� ShapeComponents�� OnComponentBeginOverlap -> ���
    /// ��Ƽĳ��Ʈ�� ���Ͽ� �� ������ DualWeapon ����
    /// </ComponentBeginOverlap>
public:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	FORCEINLINE void SetDatas(TArray<FActionData> InDatas) { Datas = InDatas; }
	FORCEINLINE void SetSkillDatas(TArray<FSkillData> InDatas) { SkillDatas = InDatas; }
	FORCEINLINE void SetSkillIndex(const int32& InIndex) { SkillIndex = InIndex; }

	FORCEINLINE void EnableCombo()  { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

	FORCEINLINE void SetWeaponStateType(const EWeaponStateType& type) { Type = type; }
	FORCEINLINE int32 GetWeaponStateType() { return static_cast<int32>(Type); }

	FORCEINLINE TSet<class AActor*> GetOverlappedActors() { return OverlappedActors; }

	UFUNCTION(BlueprintCallable)
		 USkeletalMeshComponent* GetOwnerMesh() { return OwnerCharacter->GetMesh(); }

	////// ���� �̸�
	UFUNCTION(BlueprintCallable, BlueprintPure)
		 FName GetSocketOnEquippedLeft() {    return TEXT("hand_l_weapon"); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		 FName GetSocketOnEquippedRight() {   return TEXT("hand_r_weapon"); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		 FName GetSocketOnUnequippedLeft() {  return TEXT("spine_03_l_weapon"); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		 FName GetSocketOnUnequippedRight() { return TEXT("spine_03_r_weapon"); }

public:
	UFUNCTION(BlueprintImplementableEvent)
		 void OnEquip();
	UFUNCTION(BlueprintImplementableEvent)
		 void OnUnequip();

	UFUNCTION(BlueprintCallable)
		void AttachTo(FName InSocketName);

	// HitStop ����
	UFUNCTION()
		void RestoreDilation();

	// Notify ���� �Ϲ��Լ�
public:
	// DoAction
	void OnCollision();
	void OffCollision();

	void DoAction();
	void Begin_DoAction();
	void End_DoAction();

	// DoSkill
	void CreateWeaponSkills();
	class ACDoSkill* GetWeaponSkill(const int32& num);
	void SetSkillWeaponVisibility(bool bIsVisible);

	// ����
	void OnTrail();
	void OffTrail();

#pragma region Components + OwnerCharacter
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EWeaponStateType Type;

	// Weapon ActorComponent�� / OwnerCharacter
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(BlueprintReadWrite)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

	UPROPERTY(BlueprintReadOnly)
		class UCWeaponStateComponent* WeaponState;

#pragma endregion 

protected:
	// DoAction
	TArray<FActionData> Datas;
	TSet<class AActor*> OverlappedActors;
	bool bEnable;
	bool bCanCombo;
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
};



