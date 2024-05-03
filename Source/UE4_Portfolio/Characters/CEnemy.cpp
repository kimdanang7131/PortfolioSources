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

	/** Name , Health Widget ���� ���� */
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

	// #1. ���� ����
	NameWidget->InitWidget();
	HealthWidget->InitWidget();

	// #2. NameWidget ������Ʈ �̸� ����
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameText(GetActorLabel());
	//Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetTextColor(NameColor); // ���� ����

	// #3. ������ �⺻���� -> Health ��� , Status�� �����ֱ�
	Health = Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject());
	Status->SetHealthWidget(Health);
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



