#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/CEweaponStateType.h"
#include "CWeaponStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponStateTypeChanged, EWeaponStateType, InPrevType, EWeaponStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCWeaponStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCWeaponStateComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		class UCActionDataAsset* DataWeaponA;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		class UCActionDataAsset* DataWeaponB;

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE class ACWeapon* GetCurrentWeapon() { return WeaponNow; }

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

public:
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
