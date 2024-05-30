#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Engine/DataTable.h"
#include "Enums/CEweaponStateType.h"
#include "Components/UIComponents/CUIManagementComponent.h"
#include "CSkillComponent.generated.h"

/** SkillComponent를 소유하면 DataAsset을 이 Component에다 넣고 무기 생성 후 스킬 상호작용 */
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

	/** UI와 상호작용할 SkillIcon에 대한 SkillDataTable 기반 DataTable */
	UPROPERTY(EditDefaultsOnly)
		UDataTable* SkillDT;

	/** MainSkillSlot(UI)에 Icon이 있을때만 스킬 사용 */
public:
	void SkillZ();
	void SkillX();
	void SkillC();
	void SkillV();
	void ActivateSkill(const int32& slotNum);


	/** UI 관련 상호작용 */
public:
	/** UCUIManagementComponent 재정의 */
	virtual void OpenUI() override;
	virtual void ClearUI() override;

	/** CPlayer에서 껏다키고 할 함수 */
	void ToggleSkillWindow();
	/** 스킬창 수정하고 끄는 순간 업데이트 */
	void UpdateSkillSlotDatas(const TArray<TMap<EWeaponStateType, int32>>& newSkillData);


	void Reset();

	/** Notify 관련 CWeapon과 상호작용 */
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

