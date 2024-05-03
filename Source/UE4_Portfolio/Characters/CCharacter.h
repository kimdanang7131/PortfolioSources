#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "Enums/CEStateType.h"

#include "CCharacter.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/** StateComponent�� ���� State�� �ٲ𶧸��� Delegate�� ���� Character���� �Լ��� ���� */
protected:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCInventoryComponent* Inventory;

	UPROPERTY(VisibleDefaultsOnly)
		class UCPostprocessComponent* Postprocess;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	/** EStateType�� ���� ����� �� �ֵ��� �ϴ� nullptr�� ����س��� ���� */
	UPROPERTY(EditDefaultsOnly , Category = "StateDataAsset")
		TMap<EStateType, class UCMontageDataAsset*> StateMontageMap;

	/** �ڽĵ��� �������ؼ� ����� �Լ��� , DataAsset State�� ��ϵ� �ִϸ��̼� ���� */
protected:
	virtual void Hitted() {};
	virtual void Dead();
	virtual void Dodging() {};

protected:
	/** Dead�� �� Destroyed���� �ڽĵ� ��� �ı��� ��ü �ı� */
	virtual void Destroyed();
};
