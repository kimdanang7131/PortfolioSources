#pragma once

#include "CoreMinimal.h"
#include "Characters/CCharacter.h"

#include "Engine/DataTable.h"
#include "Structs/CFItemDataTableBase.h"

#include "CTrader.generated.h"

/** 델리게이트로 Inventory에 있는 데이터를 Player에게 넘겨주어 Player UI에 넣어줘야함 */
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

	/** Player가 부딪히면 TraderInven 여는 함수 */
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


	/** Trader 영역 밖으로 나가면 Player가 정보 종합 후 이 함수 실행 -> Trader의 돈, 아이템 Update */
public:
	void UpdateTraderInvenDatas(TArray<FItemDataTableBase> InInvenFItems, const int32& InMoney);

	/** Player가 GateWay 부딪히면 Name On/Off 설정 */
	void SetNameVisibility(const bool& bVisible);
public:
	FItemTradeBeginOverlap TradeBeginDelegate;
	FItemTradeEndOverlap TradeEndDelegate;
};
