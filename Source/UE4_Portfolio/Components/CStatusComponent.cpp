#include "Components/CStatusComponent.h"
#include "Global.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Widgets/CUserWidget_MainUI.h"
#include "CPlayerController.h"
#include "Widgets/CUserWidget_Health.h"

#include "Characters/CEnemy.h"
#include "Characters/CPlayer.h"



UCStatusComponent::UCStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (!!OwnerCharacter)
		State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));		

	// 포션 디버깅용
	health  = MaxHealth;
	stamina = MaxStamina;

	GetWorld()->GetTimerManager().SetTimer(IncreaseHealthTimer, this, &UCStatusComponent::IncreaseHealth, 0.01f, true);
	GetWorld()->GetTimerManager().PauseTimer(IncreaseHealthTimer);

	GetWorld()->GetTimerManager().SetTimer(DecreaseHealthTimer, this, &UCStatusComponent::DecreaseHealth, 0.01f, true);
	GetWorld()->GetTimerManager().PauseTimer(DecreaseHealthTimer);
}

void UCStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCStatusComponent::SetHealthWidget(class UCUserWidget_Health* InHealthWidget)
{
	if (InHealthWidget == nullptr)
		return;

	HealthWidget = InHealthWidget;
	HealthWidget->Update(health, MaxHealth);
}

void UCStatusComponent::AddHealth(const float& InAmount)
{
	// #1. 현재 진행되고 있는 타이머를 정지
	GetWorld()->GetTimerManager().PauseTimer(IncreaseHealthTimer);
	
	// #2. 현재 체력 잠시 저장 -> Timer로 체력 자연스럽게 보간하기 위해
	float tempHealth = health;
	float addHealth  = health + InAmount;

	// #3. MaxHealth를 넘어가면 고정되도록
	addHealth = FMath::Clamp(addHealth, 0.f, MaxHealth);

	// #4. Timer 진행값 + 새로 들어운 값
	healthAmount += (addHealth - tempHealth);

	// #5. 최대체력이 아니라면 Timer 진행
	if (healthAmount > 0.f)
		GetWorld()->GetTimerManager().UnPauseTimer(IncreaseHealthTimer);
}

void UCStatusComponent::SubHealth(const float& InAmount)
{
	// #1. 현재 진행되고 있는 타이머를 정지
	GetWorld()->GetTimerManager().PauseTimer(DecreaseHealthTimer);

	// #2. 현재 체력 잠시 저장 -> Timer로 체력 자연스럽게 보간하기 위해
	float tempHealth = health;
	float subHealth  = health - InAmount;

	// #3. 0.f보다 낮아지면 0으로 고정
	subHealth = FMath::Clamp(subHealth, 0.f, MaxHealth);

	// #4. Timer 진행값 + 새로 들어운 값
	healthAmount += (tempHealth - subHealth);

	// #5. 최대체력이 아니라면 Timer 진행
	if (healthAmount > 0.f)
		GetWorld()->GetTimerManager().UnPauseTimer(DecreaseHealthTimer);
}

void UCStatusComponent::IncreaseHealth()
{
	// #1. 포션을 먹는 도중 맞으면 healthAmount 멈춘 후 IncreseTimer 정지
	if (State->IsHittedMode())
	{
		healthAmount = 0;
		GetWorld()->GetTimerManager().PauseTimer(IncreaseHealthTimer);
		return;
	}

	health       += INCREASE_AMOUNT;
	healthAmount -= INCREASE_AMOUNT;

	if (!!HealthWidget)
		HealthWidget->Update(health, MaxHealth);

	if (health >= MaxHealth)
	{
		health = MaxHealth;
		GetWorld()->GetTimerManager().PauseTimer(IncreaseHealthTimer);
		return;
	}

	if (healthAmount <= 0.f)
		GetWorld()->GetTimerManager().PauseTimer(IncreaseHealthTimer);
}

void UCStatusComponent::DecreaseHealth()
{
	health       -= DECREASE_AMOUNT;
	healthAmount -= DECREASE_AMOUNT;

	if (!!HealthWidget)
		HealthWidget->Update(health, MaxHealth);

	if (health < 0.f)
	{
		health = 0.f;
		GetWorld()->GetTimerManager().PauseTimer(DecreaseHealthTimer);
		return;
	}

	if (healthAmount <= 0.f)
		GetWorld()->GetTimerManager().PauseTimer(DecreaseHealthTimer);
}


void UCStatusComponent::AddStamina(const float& InAmount)
{
	// #1. Clamp때문에 필요없는데 굳이 넣는 이유는 Tick에서 
	//     이 함수를 실행하여 Clamp를 Tick마다 실행시키지 않기 위해
	if (stamina >= MaxStamina)
		return;

	stamina += InAmount;

	stamina = FMath::Clamp(stamina, 0.0f, MaxStamina);
}

void UCStatusComponent::SubStamina(const float& InAmount)
{
	stamina -= InAmount;

	stamina = FMath::Clamp(stamina, 0.0f, MaxStamina);
}

void UCStatusComponent::SetMove()
{
	bCanMove = true;
}

void UCStatusComponent::SetStop()
{
	bCanMove = false;
}

void UCStatusComponent::SetControl()
{
	bCanControl = true;
}

void UCStatusComponent::SetStopControl()
{
	bCanControl = false;
}



