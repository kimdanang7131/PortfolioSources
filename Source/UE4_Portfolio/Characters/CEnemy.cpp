#include "Characters/CEnemy.h"
#include "Global.h"
///////////////
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CPatrolRouteComponent.h"

///////////////
#include "Action/CMontageDataAsset.h"
#include "Widgets/CUserWidget_Name.h"
#include "Widgets/CUserWidget_Health.h"
///////////////
#include "Components/WidgetComponent.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UWidgetComponent>(this, &NameWidget, "NameWidget", GetMesh());
	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());

	CHelpers::CreateActorComponent<UCPatrolRouteComponent>(this, &PatrolRouteComp, "PatrolRouteComponent");

	/** Name , Health Widget 관련 설정 */
	{ 
		TSubclassOf<UCUserWidget_Name> nameClass;
		CHelpers::MyFClassFinder<UCUserWidget_Name>(&nameClass, "WidgetBlueprint'/Game/Blueprints/Widgets/WBP_Name.WBP_Name_C'");
		NameWidget->SetWidgetClass(nameClass);
		NameWidget->SetRelativeLocation(FVector(0, 0, 240));
		NameWidget->SetDrawSize(FVector2D(240, 30));
		NameWidget->SetWidgetSpace(EWidgetSpace::Screen);

		TSubclassOf<UCUserWidget_Health> healthClass;
		CHelpers::MyFClassFinder<UCUserWidget_Health>(&healthClass, "WidgetBlueprint'/Game/Blueprints/Widgets/WBP_Health.WBP_Health_C'");
		HealthWidget->SetWidgetClass(healthClass);
		HealthWidget->SetRelativeLocation(FVector(0, 0, 190));
		HealthWidget->SetDrawSize(FVector2D(120, 20));
		HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
	}
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	// #1. 위젯 실행
	NameWidget->InitWidget();
	HealthWidget->InitWidget();

	// #2. NameWidget 오브젝트 이름 설정
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameText(GetActorLabel());
	//Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetTextColor(NameColor); // 색깔 설정

	// #3. 나머지 기본설정 -> Health 등록 , Status에 보내주기
	Health = Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject());
	Status->SetHealthWidget(Health);
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



