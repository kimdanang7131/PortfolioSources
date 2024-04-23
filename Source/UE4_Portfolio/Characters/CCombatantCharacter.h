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
	UPROPERTY(VisibleDefaultsOnly)
		class UCSkillComponent* Skill;

	UPROPERTY(VisibleDefaultsOnly)
		class UCWeaponStateComponent* WeaponState;


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
		void OnWeaponStateTypeChanged(EWeaponStateType InPrevType, EWeaponStateType InNewType);

protected:
	//virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

protected:
	virtual void ToggleWeaponA();
	virtual void ToggleWeaponB();

	virtual void SkillZ() {};
	virtual void SkillX() {};
	virtual void SkillC() {};
	virtual void SkillV() {};

	virtual void Hitted()  { Super::Hitted(); }
	virtual void Dead()    { Super::Dead(); }
	virtual void Dodging() { Super::Dodging(); }
protected:

	class AController* DamageInstigator;
	float DamageValue;
};
