#include "Action/CActionDataAsset.h"
#include "Global.h"

#include "CWeapon.h"
#include "DoSkill/CDoSkill.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CWeaponStateComponent.h"

void UCActionDataAsset::CustomBeginPlay(class ACharacter* InOwnerCharacter , const EWeaponStateType& type)
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
