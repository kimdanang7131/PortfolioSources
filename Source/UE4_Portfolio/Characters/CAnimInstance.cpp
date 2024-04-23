#include "Characters/CAnimInstance.h"
#include "CAnimInstance.h"
#include "Global.h"

#include "GameFramework/Character.h"

#include "Components/CWeaponStateComponent.h"


void UCAnimInstance::NativeBeginPlay()
{
	character = Cast<ACharacter>(TryGetPawnOwner());
	NULL_RETURN(character);

	UCWeaponStateComponent* WeaponState = CHelpers::GetComponent<UCWeaponStateComponent>(character);
	NULL_RETURN(WeaponState);

	WeaponState->OnWeaponStateTypeChanged.AddDynamic(this, &UCAnimInstance::OnWeaponStateTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	NULL_RETURN(character);

	Speed = character->GetVelocity().Size2D();
	Direction = CalculateDirection(character->GetVelocity(), character->GetControlRotation());
}

void UCAnimInstance::OnWeaponStateTypeChanged(EWeaponStateType InPrevType, EWeaponStateType InNewType)
{
	WeaponStateType = InNewType;
}
