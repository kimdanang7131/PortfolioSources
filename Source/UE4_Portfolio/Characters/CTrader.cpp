#include "Characters/CTrader.h"
#include "Global.h"

#include "Components/SphereComponent.h"

#include "Components/CInventoryComponent.h"

#include "Characters/CPlayer.h"

#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Name.h"


ACTrader::ACTrader()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->SetSphereRadius(200.f);

	CHelpers::CreateComponent<UWidgetComponent>(this, &NameWidget, "NameWidget", GetMesh());

	TSubclassOf<UCUserWidget_Name> nameClass;
	CHelpers::GetClass<UCUserWidget_Name>(&nameClass, "WidgetBlueprint'/Game/Blueprints/Widgets/WBP_Name.WBP_Name_C'");
	NameWidget->SetWidgetClass(nameClass);
	NameWidget->SetRelativeLocation(FVector(0, 0, 240));
	NameWidget->SetDrawSize(FVector2D(240, 30));
	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);

	// BeginPlay -> NPC 걷기 속도 넣을려면 넣기
	// GetCharacterMovement()->MaxWalkSpeed = 200.f;
}

void ACTrader::BeginPlay()
{
	Super::BeginPlay();

	Inventory->InitFItemDataTable();
	// InvenComp로부터 가져오기

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACTrader::OnComponentBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACTrader::OnComponentEndOverlap);

	NameWidget->InitWidget();
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameText(Name);
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetTextColor(NameColor);
}

void ACTrader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

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

void ACTrader::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPlayer* CastedPlayer = Cast<ACPlayer>(OtherActor);
	if (!!CastedPlayer)
	{
		if (TradeEndDelegate.IsBound())
			TradeEndDelegate.Broadcast(this);
	}
}

void ACTrader::UpdateTraderInvenDatas(TArray<FItemDataTableBase> InInvenFItems, const int32& InMoney)
{
	Inventory->UpdateTraderInvenDatas(InInvenFItems, InMoney);
}