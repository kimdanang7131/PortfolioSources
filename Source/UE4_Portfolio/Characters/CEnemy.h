#pragma once

#include "CoreMinimal.h"
#include "Characters/CCombatantCharacter.h"
#include "CEnemy.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACEnemy : public ACCombatantCharacter
{
	GENERATED_BODY()
	

public:
	FORCEINLINE class UCUserWidget_Health* GetHealthWidget() { return Health; }
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* NameWidget;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 1;

	//private:
    //	UPROPERTY(VisibleDefaultsOnly)
    //		class UCPatrolComponent* Patrol;
    //
public:
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE uint8 GetTeamID() { return TeamID; }

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;



	virtual void Hitted() override;
	//virtual void Dead() override;
	//virtual void Dodging() override;

private:
	class UCUserWidget_Health* Health;
};
