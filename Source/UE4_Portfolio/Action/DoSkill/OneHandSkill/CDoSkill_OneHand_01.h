#pragma once

#include "CoreMinimal.h"
#include "Action/DoSkill/OneHandSkill/CDoSkill_OneHand.h"
#include "CDoSkill_OneHand_01.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACDoSkill_OneHand_01 : public ACDoSkill_OneHand
{
	GENERATED_BODY()
public:
	ACDoSkill_OneHand_01();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

	/** Effective C++ 나열자 둔갑술 enum 
	    Socket을 기준으로 좌우 일정 INTERVAL(간격)으로 표창 날리기 */
private:
	enum { INTERVAL = 50 };

	/** 표창 클래스 , 표창 개수 */
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Star")
		TSubclassOf<class ACActor_Sub_Star> starClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Star")
		int starCount = 1;

public:
	virtual void DoSkill() {}
	virtual void End_Hold() {}

	/** Notify를 통하여 표창 날리는 타이밍 지정 후 이 스킬 실행 */
	virtual void DoSubSkill() override;
public:
	virtual void Activate() override;
	virtual void Deactivate() override;

	/** 들어온 표창개수만큼 일정 간격으로 표창 소환해서 날리기 */
private:
	void SpawnStars(const int32& InStarCount);
};
