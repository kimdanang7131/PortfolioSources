#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/CFItemDataTableBase.h"
#include "CItem.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ACItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void AcivateItem(FItemDataTableBase FItem) { };

	virtual void SetItemActivate(bool InbCanActivate) { bCanActivate = InbCanActivate; }

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class AActor* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		bool bCanActivate = false;
};
