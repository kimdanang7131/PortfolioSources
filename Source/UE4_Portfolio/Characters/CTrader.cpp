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

	/** Name Widget ���� */
	{
		TSubclassOf<UCUserWidget_Name> nameClass;
		CHelpers::MyFClassFinder<UCUserWidget_Name>(&nameClass, "WidgetBlueprint'/Game/Blueprints/Widgets/WBP_Name.WBP_Name_C'");
		NameWidget->SetWidgetClass(nameClass);
		NameWidget->SetRelativeLocation(FVector(0, 0, 240));
		NameWidget->SetDrawSize(FVector2D(240, 30));
		NameWidget->SetWidgetSpace(EWidgetSpace::Screen);
	}

	// BeginPlay -> NPC �ȱ� �ӵ� �������� �ֱ�
	// GetCharacterMovement()->MaxWalkSpeed = 200.f;
}

void ACTrader::BeginPlay()
{
	Super::BeginPlay();

	// #1. ���������̺� �ִ� ������ ����
	Inventory->InitFItemDataTable();

	// #2. Trader���� ���� ( Player�� 
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACTrader::OnComponentBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACTrader::OnComponentEndOverlap);

	// #3. NameWidget �����Ű�� �̸��� ���� �־��ֱ�
	NameWidget->InitWidget();
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameText(Name);
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetTextColor(NameColor);
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameVisibility(false);
}

void ACTrader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/** Player�� �ε����� TraderInven ���� �Լ� */
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
/** Trader ���� ������ ������ �Ʒ� UpdateTraderInvenDatas Player�� ��������� */
void ACTrader::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPlayer* CastedPlayer = Cast<ACPlayer>(OtherActor);
	if (!!CastedPlayer)
	{
		if (TradeEndDelegate.IsBound())
			TradeEndDelegate.Broadcast(this);
	}
}


/** Trader ���� ������ ������ Player�� ���� ���� �� �� �Լ� ���� -> Trader�� ��, ������ Update */
void ACTrader::UpdateTraderInvenDatas(TArray<FItemDataTableBase> InInvenFItems, const int32& InMoney)
{
	Inventory->UpdateTraderInvenDatas(InInvenFItems, InMoney);
}

/** Trader�� �̸��� ������ �������� Player�� ���Ͽ� On/Off �ϱ� */
void ACTrader::SetNameVisibility(const bool& bVisible)
{
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameVisibility(bVisible);
}


