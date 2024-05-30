#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/CEweaponStateType.h"
#include "CWeaponStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponStateTypeChanged, EWeaponStateType, InPrevType, EWeaponStateType, InNewType);

/** Weapon를 소유하고있는걸 확인하는 방식은 BP에서 등록한
    ActionDataAsset을 통해서 GetWeapon()함수를 통하여 정보를 얻음*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCWeaponStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCWeaponStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Weapons")
		class UCActionDataAsset* DataWeaponA;

	UPROPERTY(EditAnywhere, Category = "Weapons")
		class UCActionDataAsset* DataWeaponB;

public:
	UFUNCTION(BlueprintPure)
		class ACWeapon* GetCurrentWeapon();

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponStateType::Unarmed; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsOneHandMode() { return Type == EWeaponStateType::OneHand; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTwoHandMode() { return Type == EWeaponStateType::TwoHand; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsKatanaMode()  { return Type == EWeaponStateType::Katana; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsDualBladeMode() { return Type == EWeaponStateType::DualBlade; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsEquipped() { return bEquipped; }

		FORCEINLINE void SetEquipped(bool bIsEquipped) { bEquipped = bIsEquipped; }

		FORCEINLINE EWeaponStateType GetWeaponStateTypeNow() { return Type; }


	void SetUnarmedMode();
	void SetOneHandMode();
	void SetTwoHandMode();
	void SetKatanaMode();
	void SetDualBladeMode();

	UFUNCTION(BlueprintCallable, Category = "Equipment")
		void ToggleWeaponA();

	UFUNCTION(BlueprintCallable, Category = "Equipment")
		void ToggleWeaponB();

	bool CheckSameWeapon(class UCActionDataAsset* WeaponData);
	void EquipWeapon(class ACWeapon* Weapon);
	void UnequipWeapon();


private:
	void SetMode(const EWeaponStateType& InType);
	void ChangeType(const EWeaponStateType& InType );

public:
	// ACDoAction
	void DoAction();
	void Begin_DoAction();
	void End_DoAction();

	void EnableCombo();
	void DisableCombo();

	// ACWeapon
	void OnCollision();
	void OffCollision();

	void OnTrail();
	void OffTrail();

	// 무기 추가 , 삭제
	void AddWeapon(class ACWeapon* InWeapon);
	//void DeleteWeapon();

public:
	/** AnimInstance에 전달하기 위해 */
	UPROPERTY(BlueprintAssignable)
		FWeaponStateTypeChanged OnWeaponStateTypeChanged;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		TMap<int32, EWeaponStateType> IndexWeaponTypeMap;

private:
	EWeaponStateType Type;
	bool bEquipped = false;

	TArray<UCActionDataAsset*> WeaponDataAssets;
	class ACWeapon* WeaponNow;
};



//FName GetSocketNameByWeaponPos(const EWeaponStateType& InType, const bool& bIsRightHand);
