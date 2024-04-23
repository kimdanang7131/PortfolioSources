#pragma once

#include "CoreMinimal.h"
#include "Action/DoSkill/KatanaSkill/CDoSkill_Katana.h"
#include "CDoSkill_Katana_02.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACDoSkill_Katana_02 : public ACDoSkill_Katana
{
	GENERATED_BODY()
	
public:
	ACDoSkill_Katana_02();

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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UNiagaraComponent* Niagara;

public:
	
	UFUNCTION()
		void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);
public:
	//virtual void DoSkill() { }
	virtual void End_Hold() override;
	virtual void DoSubSkill() override;
public:
	virtual void Activate() override;
	virtual void Deactivate() override;
};
