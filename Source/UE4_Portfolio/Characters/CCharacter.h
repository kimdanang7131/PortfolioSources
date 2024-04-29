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
	UPROPERTY(VisibleDefaultsOnly)
		class UCInventoryComponent* Inventory;

	UPROPERTY(VisibleDefaultsOnly)
		class UCPostprocessComponent* Postprocess;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;



	UPROPERTY(EditDefaultsOnly , Category = "StateDataAsset")
		TMap<EStateType, class UCMontageDataAsset*> StateMontageMap;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed();
protected:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	virtual void Hitted();
	virtual void Dead();
	virtual void Dodging() {};

private:
	bool bIsDead = false;
};
