#pragma once

#include "CoreMinimal.h"
#include "Action/DoSkill/OneHandSkill/CDoSkill_OneHand.h"
#include "CDoSkill_OneHand_02.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACDoSkill_OneHand_02 : public ACDoSkill_OneHand
{
	GENERATED_BODY()
	
public:
	ACDoSkill_OneHand_02();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USceneComponent* Scene;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UBoxComponent* Box;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UDecalComponent* Decal;

private:
	UFUNCTION()
		void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);
public:
	virtual void DoSkill() {}
	virtual void End_Hold();

public:
	virtual void Activate() override;
	virtual void Deactivate() override;


private:
	TArray<AActor*> OverlappedActors;
};
