#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Enums/CEWeaponStateType.h"
#include "Structs/CFMontageDataBase.h"
#include "Components/CWeaponStateComponent.h"

#include "CActionDataAsset.generated.h"

UCLASS()
class UE4_PORTFOLIO_API UCActionDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	ACWeapon* CustomBeginPlay(class ACharacter* InOwnerCharacter, const EWeaponStateType& type);

public:
	FORCEINLINE TSubclassOf<class ACWeapon> GetWeaponClass() { return WeaponClass; }
	FORCEINLINE class ACWeapon*  GetWeapon() { return Weapon; } // 현재 장착중인 Weapon return
	FORCEINLINE class ACDoSkill* GetDoSkill() { return DoSkill; }

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACWeapon> WeaponClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EWeaponStateType WeaponType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FActionData> ActionDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FSkillData> SkillDatas;

	/** DA_ 로 이름 붙여서 LabelName 얻기 */
private:
	FString GetLabelName(class ACharacter* InOwnerCharacter, FString InName);

private:
	class ACWeapon*  Weapon;
	class ACDoSkill* DoSkill;
};
