#include "Components/CWeaponStateComponent.h"
#include "Global.h"

#include "GameFramework/Character.h"

#include "Action/CActionDataAsset.h"
#include "Action/CWeapon.h"
#include "CStateComponent.h"

UCWeaponStateComponent::UCWeaponStateComponent()
{
	IndexWeaponTypeMap.Add(0, EWeaponStateType::Unarmed);
	IndexWeaponTypeMap.Add(1, EWeaponStateType::OneHand);
	IndexWeaponTypeMap.Add(2, EWeaponStateType::TwoHand);
	IndexWeaponTypeMap.Add(3, EWeaponStateType::Katana);
	IndexWeaponTypeMap.Add(4, EWeaponStateType::DualBlade);
}

void UCWeaponStateComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!!OwnerCharacter)
	{
		State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);

		if (!!DataWeaponA)
		{
			WeaponDataAssets.Add(DataWeaponA);
			DataWeaponA->CustomBeginPlay(OwnerCharacter, DataWeaponA->WeaponType);
		}

		if (!!DataWeaponB)
		{
			WeaponDataAssets.Add(DataWeaponB);
			DataWeaponB->CustomBeginPlay(OwnerCharacter, DataWeaponB->WeaponType);
			CLog::Print("Equip" + DataWeaponB->GetWeapon()->GetName());

		}
	}
}

void UCWeaponStateComponent::SetUnarmedMode()
{
	ChangeType(EWeaponStateType::Unarmed);
}

void UCWeaponStateComponent::SetOneHandMode()
{
	ChangeType(EWeaponStateType::OneHand);
}

void UCWeaponStateComponent::SetTwoHandMode()
{
	ChangeType(EWeaponStateType::TwoHand);
}

void UCWeaponStateComponent::SetKatanaMode()
{
	ChangeType(EWeaponStateType::Katana);
}

void UCWeaponStateComponent::SetDualBladeMode()
{
	ChangeType(EWeaponStateType::DualBlade);
}

// 로직 B있으면 추가해야됨
void UCWeaponStateComponent::ToggleWeaponA()
{
	FALSE_RETURN(State->IsIdleMode());

	// #1. 장착 중 -> 같은 무기 -> 장착 해제
    //             -> 다른 무기 -> 장착 해제 -> 다른 무기 착용
    //
    // #2. 장착 X  -> 다른 무기 착용
	if (IsEquipped())
	{
		// #3. 현재 무기와 같을 경우 해제 
		
		if (CheckSameWeapon(DataWeaponA))
		{
			UnequipWeapon();
		}
		// #4. 다를 경우 WeaponB를 들고 있었기 때문에 해제 후 WeaponA 장착
		else
		{
			UnequipWeapon();
			EquipWeapon(DataWeaponA->GetWeapon());
		}
		
	}
	else
	{
		EquipWeapon(DataWeaponA->GetWeapon());
	}
}

void UCWeaponStateComponent::ToggleWeaponB()
{
	FALSE_RETURN(State->IsIdleMode());

	CLog::Print(DataWeaponB->GetWeapon()->GetName());
	// #1. 장착 중 -> 같은 무기 -> 장착 해제
	//             -> 다른 무기 -> 장착 해제 -> 다른 무기 착용
	//
	// #2. 장착 X  -> 다른 무기 착용
	if (IsEquipped())
	{
		if (CheckSameWeapon(DataWeaponB))
		{
			UnequipWeapon();
		}
		else
		{
			UnequipWeapon();
			EquipWeapon(DataWeaponB->GetWeapon());
		}
	}
	else
	{
		EquipWeapon(DataWeaponB->GetWeapon());
	}
}


bool UCWeaponStateComponent::CheckSameWeapon(class UCActionDataAsset* WeaponData)
{
	return (WeaponNow == WeaponData->GetWeapon());
}

void UCWeaponStateComponent::EquipWeapon(class ACWeapon* Weapon)
{
	NULL_RETURN(Weapon);

	SetMode(static_cast<EWeaponStateType>(Weapon->GetWeaponStateType()));
	SetEquipped(true);

	WeaponNow = Weapon;

	GetCurrentWeapon()->OnEquip();
}

void UCWeaponStateComponent::UnequipWeapon()
{
	TRUE_RETURN(IsUnarmedMode());
	NULL_RETURN(GetCurrentWeapon());

	GetCurrentWeapon()->OnUnequip();

	SetUnarmedMode();
	SetEquipped(false);

	WeaponNow = nullptr;
}

void UCWeaponStateComponent::SetMode(const EWeaponStateType& InType)
{
	switch (InType)
	{
	case EWeaponStateType::Unarmed:
		SetUnarmedMode();
		break;
	case EWeaponStateType::OneHand:
		SetOneHandMode();
		break;
	case EWeaponStateType::TwoHand:
		SetTwoHandMode();
		break;
	case EWeaponStateType::Katana:
		SetKatanaMode();
		break;
	case EWeaponStateType::DualBlade:
		SetDualBladeMode();
		break;
	default:
		break;
	}
}


void UCWeaponStateComponent::ChangeType(const EWeaponStateType& InType)
{
	EWeaponStateType prevType = Type;
	Type = InType;

	if (OnWeaponStateTypeChanged.IsBound())
		OnWeaponStateTypeChanged.Broadcast(prevType,Type);
}


// ACDoAction
void UCWeaponStateComponent::DoAction()
{
	TRUE_RETURN(IsUnarmedMode());
	NULL_RETURN(GetCurrentWeapon());
	GetCurrentWeapon()->DoAction();
}

void UCWeaponStateComponent::Begin_DoAction()
{
	TRUE_RETURN(IsUnarmedMode());
	NULL_RETURN(GetCurrentWeapon());
	GetCurrentWeapon()->Begin_DoAction();
}

void UCWeaponStateComponent::End_DoAction()
{
	TRUE_RETURN(IsUnarmedMode());
	NULL_RETURN(GetCurrentWeapon());
	GetCurrentWeapon()->End_DoAction();
}

void UCWeaponStateComponent::EnableCombo()
{
	TRUE_RETURN(IsUnarmedMode());
	NULL_RETURN(GetCurrentWeapon());
	GetCurrentWeapon()->EnableCombo();
}

void UCWeaponStateComponent::DisableCombo()
{
	TRUE_RETURN(IsUnarmedMode());
	NULL_RETURN(GetCurrentWeapon());
	GetCurrentWeapon()->DisableCombo();
}

// Weapon
void UCWeaponStateComponent::OnCollision()
{
	TRUE_RETURN(IsUnarmedMode());
	NULL_RETURN(GetCurrentWeapon());
	GetCurrentWeapon()->OnCollision();
}

void UCWeaponStateComponent::OffCollision()
{
	TRUE_RETURN(IsUnarmedMode());
	NULL_RETURN(GetCurrentWeapon());
	GetCurrentWeapon()->OffCollision();
}

void UCWeaponStateComponent::OnTrail()
{
	TRUE_RETURN(IsUnarmedMode());
	NULL_RETURN(GetCurrentWeapon());
	GetCurrentWeapon()->OnTrail();
}

void UCWeaponStateComponent::OffTrail()
{
	TRUE_RETURN(IsUnarmedMode());
	NULL_RETURN(GetCurrentWeapon());
	GetCurrentWeapon()->OffTrail();
}



//FName UCWeaponStateComponent::GetSocketNameByWeaponPos(const EWeaponStateType& InType, const bool& bIsRightHand)
//{
//
//	if (EWeaponStateType::Unarmed == InType)
//		return bIsRightHand ? socketonUnequippedRight : socketonUnequippedLeft;
//	else
//		return socketOnEquipped;
//}