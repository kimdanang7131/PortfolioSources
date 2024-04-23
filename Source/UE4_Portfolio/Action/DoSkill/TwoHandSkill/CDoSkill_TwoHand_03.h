#pragma once

#include "CoreMinimal.h"
#include "Action/DoSkill/TwoHandSkill/CDoSkill_TwoHand.h"
#include "CDoSkill_TwoHand_03.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACDoSkill_TwoHand_03 : public ACDoSkill_TwoHand
{
	GENERATED_BODY()
	
public:
	ACDoSkill_TwoHand_03();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UParticleSystemComponent* Effect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UBoxComponent* Box;

private:
	UFUNCTION()
		void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);


public:
	virtual void DoSkill() {}
	virtual void End_Hold() {}
	virtual void DoSubSkill() override;

public:
	virtual void Activate() override;
	virtual void Deactivate() override;
};
