#include "Action/CActionDataAsset.h"
#include "Global.h"
/////////////////////
#include "CWeapon.h"
#include "DoSkill/CDoSkill.h"
#include "Characters/CCombatantCharacter.h"
/////////////////////
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

/** CWeaponStateComponent�� CSkillComponent���� ������ �Լ� */
ACWeapon* UCActionDataAsset::CustomBeginPlay(class ACharacter* InOwnerCharacter , const EWeaponStateType& type)
{
	FTransform transform;

	// #1. Spawn��Ű�� ���� DataAsset�� ���� ������ ��Ͻ�Ű�� ����
	if (!!WeaponClass)
	{
		Weapon = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACWeapon>(WeaponClass, transform, InOwnerCharacter);
		Weapon->SetActorLabel(GetLabelName(InOwnerCharacter, "_Weapon"));
		Weapon->SetDatas(ActionDatas);
		Weapon->SetSkillDatas(SkillDatas);
		Weapon->SetWeaponStateType(type);
		UGameplayStatics::FinishSpawningActor(Weapon, transform);

		// #2. DoAction , DoSkill�� �Ҷ����� Cweapon�� �ִ� Delegate ���� LaunchAmount�� CombatantCharacter�� broadCast���ֱ����� ���
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

/** DA_ �� �̸� �ٿ��� LabelName ��� */
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
