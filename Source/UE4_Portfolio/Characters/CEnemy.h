#pragma once

#include "CoreMinimal.h"
#include "Characters/CCombatantCharacter.h"
#include "CEnemy.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACEnemy : public ACCombatantCharacter
{
	GENERATED_BODY()

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE uint8 GetTeamID() { return TeamID; }
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE class UCUserWidget_Health* GetHealthWidget() { return Health; }

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* NameWidget;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UWidgetComponent* HealthWidget;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UCPatrolRouteComponent* PatrolRouteComp;


	/** GenericTeamID ������ AIController���� , �����̺��Ʈ�� �������� ����*/
private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 1;

private:
	/** Status�� �Ѱ��ֱ� ���� Widget */
	class UCUserWidget_Health* Health;
};
