#pragma once

#include "CoreMinimal.h"
#include "Characters/CCharacter.h"
#include "CCombatantCharacter.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACCombatantCharacter : public ACCharacter
{
	GENERATED_BODY()

public:
	ACCombatantCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	//virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:
	/** DYNAMIC DELEGATE�� �ƴ϶� UFUNCTION�� �ʿ���� 
	    DoAction , DoSkill�� �Ҷ����� �����             */
	FORCEINLINE const float& GetLaunchAmount() { return launchAmount; }
	FORCEINLINE void SetLaunchAmount(const float& inLaunchAmount) { launchAmount = inLaunchAmount; }


protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCSkillComponent* Skill;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UCWeaponStateComponent* WeaponState;


	/** Enemy , Player ���������� ������ �Լ��� */
protected:
	virtual void ToggleWeaponA();
	virtual void ToggleWeaponB();

	virtual void SkillZ() {};
	virtual void SkillX() {};
	virtual void SkillC() {};
	virtual void SkillV() {};
	
	/** DataAsset State�� ��ϵ� Animation�� ���� */
protected:
	virtual void Hitted() override;
	virtual void Dead()    override;
	virtual void Dodging() override;

	/** �ǰ� ���� ���ϱ� */
protected:
	int32 GetHitDirection(const FVector& start , const FVector& end , float& outAngle ,bool& outbIsLeft);

protected:
	/** TakeDamage�� Update�Ǵ� ������ */
	class AController* DamageInstigator;
	float DamageValue;
	float launchAmount = 1.f;

	float hitAngle = 0.f;
};
