#pragma once

#include "CoreMinimal.h"
#include "Characters/CCharacter.h"

#include "Engine/DataTable.h"
#include "Structs/CFItemDataTableBase.h"

#include "CTrader.generated.h"

/** ��������Ʈ�� Inventory�� �ִ� �����͸� Player���� �Ѱ��־� Player UI�� �־������ */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemTradeBeginOverlap, const TArray<FItemDataTableBase>& , InTraderFItems , const int32&  , InMoney);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemTradeEndOverlap , ACTrader*, InTrader);

UCLASS()
class UE4_PORTFOLIO_API ACTrader : public ACCharacter
{
	GENERATED_BODY()

public:
	ACTrader();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

	/** Player�� �ε����� TraderInven ���� �Լ� */
public:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UCPatrolRouteComponent* PatrolRouteComp;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* NameWidget;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		FString Name;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		FLinearColor NameColor;


	/** Trader ���� ������ ������ Player�� ���� ���� �� �� �Լ� ���� -> Trader�� ��, ������ Update */
public:
	void UpdateTraderInvenDatas(TArray<FItemDataTableBase> InInvenFItems, const int32& InMoney);

	/** Player�� GateWay �ε����� Name On/Off ���� */
	void SetNameVisibility(const bool& bVisible);
public:
	FItemTradeBeginOverlap TradeBeginDelegate;
	FItemTradeEndOverlap TradeEndDelegate;
};
