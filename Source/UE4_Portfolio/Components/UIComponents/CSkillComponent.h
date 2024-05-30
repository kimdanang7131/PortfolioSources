#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Engine/DataTable.h"
#include "Enums/CEweaponStateType.h"
#include "Components/UIComponents/CUIManagementComponent.h"
#include "CSkillComponent.generated.h"

/** SkillComponent�� �����ϸ� DataAsset�� �� Component���� �ְ� ���� ���� �� ��ų ��ȣ�ۿ� */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCSkillComponent : public UCUIManagementComponent
{
	GENERATED_BODY()

public:	
	UCSkillComponent();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		TMap<EWeaponStateType, class UCActionDataAsset*> SkillWeaponDatas;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCWeaponStateComponent* WeaponState;

	/** UI�� ��ȣ�ۿ��� SkillIcon�� ���� SkillDataTable ��� DataTable */
	UPROPERTY(EditDefaultsOnly)
		UDataTable* SkillDT;

	/** MainSkillSlot(UI)�� Icon�� �������� ��ų ��� */
public:
	void SkillZ();
	void SkillX();
	void SkillC();
	void SkillV();
	void ActivateSkill(const int32& slotNum);


	/** UI ���� ��ȣ�ۿ� */
public:
	/** UCUIManagementComponent ������ */
	virtual void OpenUI() override;
	virtual void ClearUI() override;

	/** CPlayer���� ����Ű�� �� �Լ� */
	void ToggleSkillWindow();
	/** ��ųâ �����ϰ� ���� ���� ������Ʈ */
	void UpdateSkillSlotDatas(const TArray<TMap<EWeaponStateType, int32>>& newSkillData);


	void Reset();

	/** Notify ���� CWeapon�� ��ȣ�ۿ� */
public:
	bool CheckCanExecute();

	void OnAttackByTimer();
	void OffAttackByTimer();

	void DoSubSkill();

	void End_DoSkill();
	void End_Hold();

	void Begin_FromSlash();
	void End_FromSlash();

private:
	TMap<EWeaponStateType, class ACWeapon*> SkillWeapons;
	TArray<TMap<EWeaponStateType,int32>> SkillSlotDatas;

	class ACWeapon* CacheWeapon    = nullptr;
	class ACWeapon* SkillWeaponNow = nullptr;

	int32 WeaponSkillIndex = -1;
	
};

