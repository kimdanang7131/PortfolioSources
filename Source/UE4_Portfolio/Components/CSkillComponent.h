#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Enums/CEweaponStateType.h"
#include "Components/CUIManagementComponent.h"
#include "CSkillComponent.generated.h"


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

	// 나중에 플레이어 UI와 연결해서 사용
	void SkillZ();
	void SkillX();
	void SkillC();
	void SkillV();
	void ActivateSkill(const int32& slotNum);

	///
	void ToggleSkillWindow();
	void UpdateSkillSlotDatas(const TArray<TMap<EWeaponStateType, int32>>& newSkillData);

	virtual void OpenUI() override;
	virtual void ClearUI() override;

	// Notify 연관
public:
	bool CheckCanExecute();

	void OnAttackByTimer();
	void OffAttackByTimer();

	void DoSubSkill();

	void End_DoSkill();
	void End_Hold();

	void Begin_FromSlash();
	void End_FromSlash();
	
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		TMap<EWeaponStateType, class UCActionDataAsset*> SkillWeaponDatas;


	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCWeaponStateComponent* WeaponState;

	UPROPERTY(EditDefaultsOnly)
		UDataTable* SkillDT;

private:
	TMap<EWeaponStateType, class ACWeapon*> SkillWeapons;
	TArray<TMap<EWeaponStateType,int32>> SkillSlotDatas;

	class ACWeapon* CacheWeapon = nullptr;
	class ACWeapon* SkillWeaponNow = nullptr;

	int32 WeaponSkillIndex = -1;
};

