#pragma once

#include "CoreMinimal.h"
#include "Characters/CCharacter.h"
#include "Engine/DataTable.h"
#include "Structs/CFItemDataTableBase.h"
#include "CTrader.generated.h"

// ��������Ʈ�� Inventory�� �ִ� �����͸� Player���� �Ѱ��־� Player UI�� �־������
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemTradeBeginOverlap, const TArray<FItemDataTableBase>& , InTraderFItems , const int32&  , InMoney);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemTradeEndOverlap , ACTrader*, InTrader);


UCLASS()
class UE4_PORTFOLIO_API ACTrader : public ACCharacter
{
	GENERATED_BODY()

public:
	ACTrader();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UCPatrolRouteComponent* PatrolRouteComp;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* NameWidget;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		FString Name;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		FLinearColor NameColor;

public:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:
	void UpdateTraderInvenDatas(TArray<FItemDataTableBase> InInvenFItems, const int32& InMoney);
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;



	FItemTradeBeginOverlap TradeBeginDelegate;
	FItemTradeEndOverlap TradeEndDelegate;
};
