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

	/** Effective C++ ������ �а��� enum 
	    Socket�� �������� �¿� ���� INTERVAL(����)���� ǥâ ������ */
private:
	enum { INTERVAL = 50 };

	/** ǥâ Ŭ���� , ǥâ ���� */
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Star")
		TSubclassOf<class ACActor_Sub_Star> starClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Star")
		int starCount = 1;

public:
	virtual void DoSkill() {}
	virtual void End_Hold() {}

	/** Notify�� ���Ͽ� ǥâ ������ Ÿ�̹� ���� �� �� ��ų ���� */
	virtual void DoSubSkill() override;
public:
	virtual void Activate() override;
	virtual void Deactivate() override;

	/** ���� ǥâ������ŭ ���� �������� ǥâ ��ȯ�ؼ� ������ */
private:
	void SpawnStars(const int32& InStarCount);
};
