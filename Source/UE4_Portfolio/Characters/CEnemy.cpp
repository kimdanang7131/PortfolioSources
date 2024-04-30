#include "Characters/CEnemy.h"
#include "Global.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Name.h"
#include "Widgets/CUserWidget_Health.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"

#include "Action/CMontageDataAsset.h"
#include "Components/CPatrolRouteComponent.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UWidgetComponent>(this, &NameWidget, "NameWidget", GetMesh());
	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());
	
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

	CHelpers::CreateActorComponent<UCPatrolRouteComponent>(this, &PatrolRouteComp, "PatrolRouteComponent");
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	NameWidget->InitWidget();
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameText(GetActorLabel());

	HealthWidget->InitWidget();

	Health = Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject());
	Status->SetHealthWidget(Health);
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACEnemy::Hitted()
{
	Status->SubHealth(DamageValue);

	if (Status->GetHealth() - DamageValue <= 0.0f)
	{
		DamageValue = 0.0f;
		State->SetDeadMode();
		return;
	}

	DamageValue = 0.0f;

	UCMontageDataAsset** MontageData = StateMontageMap.Find(State->GetStateNow());
	FALSE_RETURN(*MontageData);

	Status->SetStop();

	PlayAnimMontage((*MontageData)->GetMontageData(0).AnimMontage, (*MontageData)->GetMontageData(0).PlayRatio, (*MontageData)->GetMontageData(0).StartSection);

	FVector start = GetActorLocation();
	FVector target = DamageInstigator->GetPawn()->GetActorLocation();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
	DamageInstigator = NULL;

	FVector direction = target - start;
	direction.Normalize();

	LaunchCharacter(-direction * LaunchAmount, true, false);
}

//void ACEnemy::Dead()
//{
//}
//
//void ACEnemy::Dodging()
//{
//}

