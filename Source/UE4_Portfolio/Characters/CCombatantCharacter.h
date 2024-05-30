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
	/** DYNAMIC DELEGATE가 아니라서 UFUNCTION할 필요없음 
	    DoAction , DoSkill을 할때마다 실행됨             */
	FORCEINLINE const float& GetLaunchAmount() { return launchAmount; }
	FORCEINLINE void SetLaunchAmount(const float& inLaunchAmount) { launchAmount = inLaunchAmount; }


protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCSkillComponent* Skill;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UCWeaponStateComponent* WeaponState;


	/** Enemy , Player 공통적으로 실행할 함수들 */
protected:
	virtual void ToggleWeaponA();
	virtual void ToggleWeaponB();

	virtual void SkillZ() {};
	virtual void SkillX() {};
	virtual void SkillC() {};
	virtual void SkillV() {};
	
	/** DataAsset State에 등록된 Animation을 실행 */
protected:
	virtual void Hitted() override;
	virtual void Dead()    override;
	virtual void Dodging() override;

	/** 피격 방향 구하기 */
protected:
	int32 GetHitDirection(const FVector& start , const FVector& end , float& outAngle ,bool& outbIsLeft);

protected:
	/** TakeDamage시 Update되는 변수들 */
	class AController* DamageInstigator;
	float DamageValue;
	float launchAmount = 1.f;

	float hitAngle = 0.f;
};
