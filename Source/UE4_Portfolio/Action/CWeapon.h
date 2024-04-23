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

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USceneComponent* Scene;

public:
	FORCEINLINE void SetDatas(TArray<FActionData> InDatas) { Datas = InDatas; }
	FORCEINLINE void SetSkillDatas(TArray<FSkillData> InDatas) { SkillDatas = InDatas; }
	FORCEINLINE void SetSkillIndex(const int32& InIndex) { SkillIndex = InIndex; }
	class ACDoSkill* GetWeaponSkill(const int32& num) { return WeaponSkills[num]; }
	void SetSkillWeaponVisibility(bool bIsVisible);

	FORCEINLINE TSet<class AActor*> GetOverlappedActors() { return OverlappedActors; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE USkeletalMeshComponent* GetOwnerMesh() { return OwnerCharacter->GetMesh(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		 FName GetSocketOnEquippedLeft() { return TEXT("hand_l_weapon"); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		 FName GetSocketOnEquippedRight() { return TEXT("hand_r_weapon"); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		 FName GetSocketOnUnequippedLeft() { return TEXT("spine_03_l_weapon"); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		 FName GetSocketOnUnequippedRight() { return TEXT("spine_03_r_weapon"); }

public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

	UFUNCTION(BlueprintImplementableEvent)
		 void OnEquip();
	UFUNCTION(BlueprintImplementableEvent)
		 void OnUnequip();

	UFUNCTION(BlueprintCallable)
		void AttachTo(FName InSocketName);


	FORCEINLINE void SetWeaponStateType(const EWeaponStateType& type) { Type = type; }
	FORCEINLINE int32 GetWeaponStateType() { return static_cast<int32>(Type); }

	// 멀티캐스트 델리게이트를 이용하여 ShapeComponents의 OnComponentBeginOverlap -> 등록
	// 멀티캐스트를 통하여 한 이유는 DualWeapon 때문
public:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlaapedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:
	void OnCollision();
	void OffCollision();

	void OnTrail();
	void OffTrail();

public:
	void DoAction();
	void Begin_DoAction();
	void End_DoAction();

	void RestoreDilation();

	void CreateWeaponSkills();
	//void PlayWeaponSkill()
#pragma region Components + OwnerCharacter

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EWeaponStateType Type;

	// Weapon ActorComponent들 / OwnerCharacter
protected:
	UPROPERTY(BlueprintReadWrite)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

	UPROPERTY(BlueprintReadOnly)
		class UCWeaponStateComponent* WeaponState;

#pragma endregion 


private:
	TArray<class USkeletalMeshComponent*> MeshComponents;
	TArray<class UShapeComponent*> ShapeComponents;
	TArray<class UParticleSystemComponent*> ParticleSystemComponents;

protected:
	// DoSkill
	TArray<FSkillData> SkillDatas;
	TArray<class ACDoSkill*> WeaponSkills;

	// DoAction
	TArray<FActionData> Datas;
	TSet<class AActor*> OverlappedActors;

	bool bEnable;
	bool bCanCombo;

	int32 Index = 0;
	int32 SkillIndex = 0;


	/// test용
	int32 n = 0;
};



