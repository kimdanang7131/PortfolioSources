#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

/** Health , Stamina,  �̵�/���콺ȸ�� Lock ���� , Movement���� ��� */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	const float INCREASE_AMOUNT = 0.3f;
	const float DECREASE_AMOUNT = 0.9f;
public:	
	UCStatusComponent();
protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth()    { return health; }

	FORCEINLINE float GetMaxStamina() { return MaxStamina; }
	FORCEINLINE float GetStamina()    { return stamina; }

	FORCEINLINE float GetWalkSpeed()   { return WalkSpeed; }
	FORCEINLINE float GetRunSpeed()    { return RunSpeed; }
	FORCEINLINE float GetSprintSpeed() { return SprintSpeed; }

	FORCEINLINE float GetHorizontalLookRate() { return HorizontalLookRate; }
	FORCEINLINE float GetVerticalLookRate()   { return VerticalLookRate; }

	FORCEINLINE bool CheckCanMove()    { return bCanMove; }
	FORCEINLINE bool CheckCanControl() { return bCanControl; }

	/** Stamina�� ����� �� DoAction or DoSkill�� �������� �ľ� */
	FORCEINLINE bool CheckCanDoAction(const float& InStamina) { return (stamina - InStamina) > 0; }


	/** Timer�� �̿��Ͽ� ������ Health */
public:
	UFUNCTION()
		void IncreaseHealth();

	UFUNCTION()
		void DecreaseHealth();

private:
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 120.f;

	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float WalkSpeed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float RunSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float SprintSpeed = 800.f;

	UPROPERTY(EditAnywhere, Category = "LookRate")
		float HorizontalLookRate = 45;

	UPROPERTY(EditAnywhere, Category = "LookRate")
		float VerticalLookRate = 45;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCStateComponent* State;

public:
	void SetHealthWidget(class UCUserWidget_Health* InHealthWidget);
	void SetMovementSpeed(const float& InSpeed);

	void AddHealth(const float& InAmount);
	void SubHealth(const float& InAmount);

	void AddStamina(const float& InAmount);
	void SubStamina(const float& InAmount);

	void SetMove();
	void SetStop();
	void LockMoveInput(const bool& bIgnoreInput);

	void SetControl();
	void SetStopControl();
	void LockMouseMove(const bool& bIgnoreLook);

	/** Health ���� */
private:
	class UCUserWidget_Health* HealthWidget;
	FTimerHandle IncreaseHealthTimer;
	FTimerHandle DecreaseHealthTimer;
	float healthAmount = 0.f;
	float health;
	float stamina;
	
	/** �̵�, ���콺 ȸ�� ��ȣ�ۿ� ���� */
private:
	bool bCanMove    = false;
	bool bCanControl = false;
	AController* LockController;
};
