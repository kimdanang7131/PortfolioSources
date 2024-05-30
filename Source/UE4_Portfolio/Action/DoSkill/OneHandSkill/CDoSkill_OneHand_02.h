#pragma once

#include "CoreMinimal.h"
#include "Action/DoSkill/OneHandSkill/CDoSkill_OneHand.h"
#include "CDoSkill_OneHand_02.generated.h"

/** �ٴڿ� Skill Range Decal �� �� Ȧ�� ������ ������ �ִ� [ Actor�鿡�� ������ �� ���� �� Actor�� �̵�  ] */
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

private:
	UFUNCTION()
		void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

#pragma region UPROPERTIES
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USceneComponent* Scene;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UBoxComponent* Box;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UDecalComponent* Decal;
#pragma endregion

public:
	virtual void DoSkill() {}
	virtual void End_Hold();
public:
	virtual void Activate() override;
	virtual void Deactivate() override;

	/** Skill Range�� �ִ� Actors�� ���������� ����� �־�� �ؼ� TArray�� ���� */
private:
	TArray<AActor*> OverlappedActors;
};
