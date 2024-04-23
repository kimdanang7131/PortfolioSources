#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Structs/CFMontageDataBase.h"
#include "CMontageDataAsset.generated.h"

UCLASS()
class UE4_PORTFOLIO_API UCMontageDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	//void CustomBeginPlay(class ACharacter* InOwnerCharacter, const EWeaponStateType& type);
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FMontageData> MontageDatas;

public:
	UFUNCTION(BlueprintCallable)
		TArray<FMontageData> GetMontageDatas() const;

	UFUNCTION(BlueprintCallable)
		FMontageData GetMontageData(const int& idx);
};

