#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CWeaponStateComponent.h"
#include "Action/CActionDataAsset.h"

#include "CWeapon.generated.h"

/** DoAction할때 때리는 Character에게 얼마만큼 Launch할지에 대한 정보 넘겨줄 델리게이트 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLaunchCharacter, const float& inLaunchAmount);

UCLASS()
class UE4_PORTFOLIO_API ACWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACWeapon();

protected:
	virtual void BeginPlay() override;

	/** 멀티캐스트 델리게이트를 이용하여 BP에 설정한 모든 ShapeComponents의 OnComponentBeginOverlap -> 등록 
		멀티캐스트를 통하여 한 이유는 DualWeapon 때문 */
public:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** DoAction할때 때리는 Character에게 얼마만큼 Launch할지에 대한 정보 넘겨줄 델리게이트 */
public:
	FOnLaunchCharacter OnLaunchCharacterDelegate;

public:
	/** ActionData로부터 SkillData 등록, Index는 매번 Skill사용시 호출, Skill DoAction과 연관되어있는 부분 스킬사용시 호출 */
	FORCEINLINE void SetSkillDatas(TArray<FSkillData> InDatas) { SkillDatas = InDatas; }
	FORCEINLINE void SetSkillIndex(const int32& InIndex) { SkillIndex = InIndex; }
	FORCEINLINE void ExecuteSkillLaunchAmountBroadCast(const float& inLaunchAmount) { OnLaunchCharacterDelegate.Broadcast(inLaunchAmount); }
	FORCEINLINE TSet<class AActor*> GetOverlappedActors() { return OverlappedActors; }

	/** ActionData로부터 WeaponData 등록 */
	FORCEINLINE void SetDatas(TArray<FActionData> InDatas) { Datas = InDatas; }
	FORCEINLINE void SetWeaponStateType(const EWeaponStateType& type) { Type = type; }
	FORCEINLINE int32 GetWeaponStateType() { return static_cast<int32>(Type); }

	/** Notify */
	FORCEINLINE void EnableCombo()  { bCanCombo = true; }
	FORCEINLINE void DisableCombo() { bCanCombo = false; }

	/** 블루프린트에서 사용할 함수들 */
public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnEquip();
	UFUNCTION(BlueprintImplementableEvent)
		void OnUnequip();

	UFUNCTION(BlueprintCallable)
		void AttachTo(FName InSocketName);

public:
	/** Hitstop 사용 시*/
	UFUNCTION()
		void RestoreDilation();

#pragma region SocketName , OwnerMesh
public:
	/** 소켓 이름, 블루프린트에서 사용 모음 */
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

	// Weapon ActorComponent들 / OwnerCharacter
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


	/** 무기 피격 시 사운드*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effect")
		class UParticleSystem* BloodEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effect")
		class USoundCue* HitSound;

#pragma endregion 

	// Notify 관련 일반함수
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

	// 공용
	void OnTrail();
	void OffTrail();

	// 콤보 도중 피격시 Index초기화 및 설정
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
	// Property 관련
	TArray<class USkeletalMeshComponent*> MeshComponents;
	TArray<class UShapeComponent*> ShapeComponents;
	TArray<class UParticleSystemComponent*> ParticleSystemComponents;
	//TSubclassOf<class UCMatineeCameraShake> camShake;
};



