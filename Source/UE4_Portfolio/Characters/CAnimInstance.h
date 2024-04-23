#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CWeaponStateComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class UE4_PORTFOLIO_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
		void OnWeaponStateTypeChanged(EWeaponStateType InPrevType, EWeaponStateType InNewType);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EWeaponStateType WeaponStateType;

private:
	class ACharacter* character;
};
