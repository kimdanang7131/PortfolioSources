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


	/** WeaponStateComponent로부터 연결된 Delegate를 통해 변경됨 -> 아직 안씀 ( 장비 장착해제 관련 애니메이션 안써서 )*/
private:
	UFUNCTION()
		void OnWeaponStateTypeChanged(EWeaponStateType InPrevType, EWeaponStateType InNewType);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EWeaponStateType WeaponStateType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Direction;


	/** 이 애니메이션의 주체 */
private:
	class ACharacter* OwnerCharacter;
};
