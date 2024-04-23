#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UCStatusComponent();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxStamina = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
		float WalkSpeed = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
		float RunSpeed = 400.f;

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
		float SprintSpeed = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "LookRate")
		float HorizontalLookRate = 45;

	UPROPERTY(EditDefaultsOnly, Category = "LookRate")
		float VerticalLookRate = 45;

public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth() { return health; }

	FORCEINLINE float GetMaxStamina() { return MaxStamina; }
	FORCEINLINE float GetStamina() { return stamina; }

	FORCEINLINE float GetWalkSpeed() { return WalkSpeed; }
	FORCEINLINE float GetRunSpeed()  { return RunSpeed; }
	FORCEINLINE float GetSprintSpeed() { return SprintSpeed; }

	FORCEINLINE float GetHorizontalLookRate() { return HorizontalLookRate; }
	FORCEINLINE float GetVerticalLookRate() { return VerticalLookRate; }

	FORCEINLINE bool CanMove() { return bCanMove; }
	FORCEINLINE bool CanControl() { return bCanControl; }

	FORCEINLINE bool CheckCanDoAction(const float& InStamina) { return (stamina - InStamina) > 0; }


protected:
	virtual void BeginPlay() override;

public:
	void AddHealth(const float& InAmount);
	void SubHealth(const float& InAmount);

	void AddStamina(const float& InAmount);
	void SubStamina(const float& InAmount);

	void SetMove();
	void SetStop();

	void SetControl();
	void SetStopControl();

	UFUNCTION()
		void IncreaseHealth();

	UFUNCTION()
		void DecreaseHealth();

	void SetHealthWidget(class UCUserWidget_Health* InHealthWidget);
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCStateComponent* State;

	class UCUserWidget_Health* HealthWidget;

private:
	const float INCREASE_AMOUNT = 0.2f;
	const float DECREASE_AMOUNT = 0.5f;

	FTimerHandle IncreaseHealthTimer;
	FTimerHandle DecreaseHealthTimer;

	float healthAmount = 0.f;
private:
	float health;
	float stamina;

	bool bCanMove = false;
	bool bCanControl = false;
};
