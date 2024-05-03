#include "Action/CActionDataAsset.h"
#include "Global.h"

#include "CWeapon.h"
#include "DoSkill/CDoSkill.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CWeaponStateComponent.h"
#include "Characters/CCombatantCharacter.h"

ACWeapon* UCActionDataAsset::CustomBeginPlay(class ACharacter* InOwnerCharacter , const EWeaponStateType& type)
{
	FTransform transform;

	if (!!WeaponClass)
	{
		Weapon = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACWeapon>(WeaponClass, transform, InOwnerCharacter);
		Weapon->SetActorLabel(GetLabelName(InOwnerCharacter, "_Weapon"));
		Weapon->SetDatas(ActionDatas);
		Weapon->SetSkillDatas(SkillDatas);
		Weapon->SetWeaponStateType(type);
		UGameplayStatics::FinishSpawningActor(Weapon, transform);

		ACCombatantCharacter* CombatCharacter = Cast<ACCombatantCharacter>(InOwnerCharacter);
		if (CombatCharacter)
		{
			Weapon->OnLaunchCharacterDelegate.AddUObject(CombatCharacter, &ACCombatantCharacter::SetLaunchAmount);
		}
		return Weapon;
	}
	else
	{
		return nullptr;
	}
}

FString UCActionDataAsset::GetLabelName(ACharacter* InOwnerCharacter, FString InName)
{
	FString str;
	str.Append(InOwnerCharacter->GetActorLabel());
	str.Append("_");
	str.Append(InName);
	str.Append("_");
	str.Append(GetName().Replace(TEXT("DA_"), TEXT("")));

	return str;
}
