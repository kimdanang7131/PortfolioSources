#include "Action/CActionDataAsset.h"
#include "Global.h"
/////////////////////
#include "CWeapon.h"
#include "DoSkill/CDoSkill.h"
#include "Characters/CCombatantCharacter.h"
/////////////////////
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

/** CWeaponStateComponent와 CSkillComponent에서 실행할 함수 */
ACWeapon* UCActionDataAsset::CustomBeginPlay(class ACharacter* InOwnerCharacter , const EWeaponStateType& type)
{
	FTransform transform;

	// #1. Spawn시키기 전에 DataAsset에 관한 정보들 등록시키고 스폰
	if (!!WeaponClass)
	{
		Weapon = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACWeapon>(WeaponClass, transform, InOwnerCharacter);
		Weapon->SetActorLabel(GetLabelName(InOwnerCharacter, "_Weapon"));
		Weapon->SetDatas(ActionDatas);
		Weapon->SetSkillDatas(SkillDatas);
		Weapon->SetWeaponStateType(type);
		UGameplayStatics::FinishSpawningActor(Weapon, transform);

		// #2. DoAction , DoSkill을 할때마다 Cweapon에 있는 Delegate 통해 LaunchAmount를 CombatantCharacter로 broadCast해주기위한 등록
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

/** DA_ 로 이름 붙여서 LabelName 얻기 */
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
