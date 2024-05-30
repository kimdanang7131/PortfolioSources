#include "Characters/CAnimInstance.h"
#include "Global.h"

//////////////////
#include "Components/CWeaponStateComponent.h"
#include "CAnimInstance.h"
//////////////////
#include "GameFramework/Character.h"


/** Animinstance의 BeginPlay */
void UCAnimInstance::NativeBeginPlay()
{
	// #1. 본체를 얻어서 character에 등록 
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	NULL_RETURN(OwnerCharacter);

	UCWeaponStateComponent* WeaponState = CHelpers::GetComponent<UCWeaponStateComponent>(OwnerCharacter);
	//UCWeaponStateComponent* WeaponState = Cast<UCWeaponStateComponent>(character->GetComponentByClass(UCWeaponStateComponent::StaticClass()));
	NULL_RETURN(WeaponState);

	// #2. WeaponState가 바뀔때마다 Delegate를 통해 전달하기 위해서 ( State도 사실 해야하는데 어차피 안씀 )
	WeaponState->OnWeaponStateTypeChanged.AddDynamic(this, &UCAnimInstance::OnWeaponStateTypeChanged);
}

/** Animinstance의 UpdateAnimation */
void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	NULL_RETURN(OwnerCharacter);

	// #1. Speed는 Velocity얻어서 Length 가져오기
	//     Direction은 원래 Dot , Cross를 통해 구해야하는데 간단하게 CalculateDirection
	Speed     = OwnerCharacter->GetVelocity().Size2D();
	Direction = CalculateDirection(OwnerCharacter->GetVelocity(), FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0));
}


/** WeaponState로 연결된 Delegate 함수 */
void UCAnimInstance::OnWeaponStateTypeChanged(EWeaponStateType InPrevType, EWeaponStateType InNewType)
{
	WeaponStateType = InNewType;
}

