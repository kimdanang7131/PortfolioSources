#include "Characters/CAnimInstance.h"
#include "Global.h"

//////////////////
#include "Components/CWeaponStateComponent.h"
#include "CAnimInstance.h"
//////////////////
#include "GameFramework/Character.h"


/** Animinstance�� BeginPlay */
void UCAnimInstance::NativeBeginPlay()
{
	// #1. ��ü�� �� character�� ��� 
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	NULL_RETURN(OwnerCharacter);

	UCWeaponStateComponent* WeaponState = CHelpers::GetComponent<UCWeaponStateComponent>(OwnerCharacter);
	//UCWeaponStateComponent* WeaponState = Cast<UCWeaponStateComponent>(character->GetComponentByClass(UCWeaponStateComponent::StaticClass()));
	NULL_RETURN(WeaponState);

	// #2. WeaponState�� �ٲ𶧸��� Delegate�� ���� �����ϱ� ���ؼ� ( State�� ��� �ؾ��ϴµ� ������ �Ⱦ� )
	WeaponState->OnWeaponStateTypeChanged.AddDynamic(this, &UCAnimInstance::OnWeaponStateTypeChanged);
}

/** Animinstance�� UpdateAnimation */
void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	NULL_RETURN(OwnerCharacter);

	// #1. Speed�� Velocity�� Length ��������
	//     Direction�� ���� Dot , Cross�� ���� ���ؾ��ϴµ� �����ϰ� CalculateDirection
	Speed     = OwnerCharacter->GetVelocity().Size2D();
	Direction = CalculateDirection(OwnerCharacter->GetVelocity(), FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0));
}


/** WeaponState�� ����� Delegate �Լ� */
void UCAnimInstance::OnWeaponStateTypeChanged(EWeaponStateType InPrevType, EWeaponStateType InNewType)
{
	WeaponStateType = InNewType;
}

