#include "Characters/CTrader.h"
#include "Global.h"
////////////////////
#include "Components/UIComponents/CInventoryComponent.h"
#include "Components/CPatrolRouteComponent.h"

////////////////////
#include "Widgets/CUserWidget_Name.h"
#include "Characters/CPlayer.h"
////////////////////
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

ACTrader::ACTrader()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->SetSphereRadius(200.f);

	CHelpers::CreateComponent<UWidgetComponent>(this, &NameWidget, "NameWidget", GetMesh());
	CHelpers::CreateActorComponent<UCPatrolRouteComponent>(this, &PatrolRouteComp, "PatrolRouteComponent");

	/** Name Widget 설정 */
	{
		TSubclassOf<UCUserWidget_Name> nameClass;
		CHelpers::MyFClassFinder<UCUserWidget_Name>(&nameClass, "WidgetBlueprint'/Game/Blueprints/Widgets/WBP_Name.WBP_Name_C'");
		NameWidget->SetWidgetClass(nameClass);
		NameWidget->SetRelativeLocation(FVector(0, 0, 240));
		NameWidget->SetDrawSize(FVector2D(240, 30));
		NameWidget->SetWidgetSpace(EWidgetSpace::Screen);
	}

	// BeginPlay -> NPC 걷기 속도 넣을려면 넣기
	// GetCharacterMovement()->MaxWalkSpeed = 200.f;
}

void ACTrader::BeginPlay()
{
	Super::BeginPlay();

	// #1. 데이터테이블에 있는 아이템 설정
	Inventory->InitFItemDataTable();

	// #2. Trader영역 설정 ( Player가 
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACTrader::OnComponentBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACTrader::OnComponentEndOverlap);

	// #3. NameWidget 실행시키고 이름과 색깔 넣어주기
	NameWidget->InitWidget();
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameText(Name);
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetTextColor(NameColor);
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameVisibility(false);
}

void ACTrader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/** Player가 부딪히면 TraderInven 여는 함수 */
void ACTrader::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TRUE_RETURN(OtherActor->GetClass() == GetClass());

	ACPlayer* CastedPlayer = Cast<ACPlayer>(OtherActor);
	if (!!CastedPlayer)
	{
		if (TradeBeginDelegate.IsBound())
			TradeBeginDelegate.Broadcast(Inventory->GetFItemDatas() , Inventory->GetMoney());
	}

}
/** Trader 영역 밖으로 나가면 아래 UpdateTraderInvenDatas Player가 실행시켜줌 */
void ACTrader::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPlayer* CastedPlayer = Cast<ACPlayer>(OtherActor);
	if (!!CastedPlayer)
	{
		if (TradeEndDelegate.IsBound())
			TradeEndDelegate.Broadcast(this);
	}
}


/** Trader 영역 밖으로 나가면 Player가 정보 종합 후 이 함수 실행 -> Trader의 돈, 아이템 Update */
void ACTrader::UpdateTraderInvenDatas(TArray<FItemDataTableBase> InInvenFItems, const int32& InMoney)
{
	Inventory->UpdateTraderInvenDatas(InInvenFItems, InMoney);
}

/** Trader의 이름을 마을에 들어왔을때 Player를 통하여 On/Off 하기 */
void ACTrader::SetNameVisibility(const bool& bVisible)
{
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameVisibility(bVisible);
}


