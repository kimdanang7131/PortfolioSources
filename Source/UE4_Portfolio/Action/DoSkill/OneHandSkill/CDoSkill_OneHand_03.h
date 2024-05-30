#pragma once

#include "CoreMinimal.h"
#include "Action/DoSkill/OneHandSkill/CDoSkill_OneHand.h"
#include "CDoSkill_OneHand_03.generated.h"

/** 바닥에 Skill Range Decal 깐 후 홀드 끝나면 범위에 있는 [ 가장 가까운 Actor로 이동 후 콤보 공격 애니메이션 ] */
UCLASS()
class UE4_PORTFOLIO_API ACDoSkill_OneHand_03 : public ACDoSkill_OneHand
{
	GENERATED_BODY()
public:
	ACDoSkill_OneHand_03();
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
	virtual void End_Hold() override;
public:
	virtual void Activate() override;
	virtual void Deactivate() override;

	/** Skill Range에 있는 Actors들 지속적으로 지우고 넣어야 해서 TArray로 설정 
	    EndHold 후 Actor 하나로 이동해야 하기 때문에 따로 변수 생성 */
private:
	TArray<AActor*> OverlappedActors;
	AActor* TargetActor;
};
