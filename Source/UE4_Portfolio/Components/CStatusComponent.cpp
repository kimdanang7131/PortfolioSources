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

	// ���� ������
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
	// #1. ���� ����ǰ� �ִ� Ÿ�̸Ӹ� ����
	GetWorld()->GetTimerManager().PauseTimer(IncreaseHealthTimer);
	
	// #2. ���� ü�� ��� ���� -> Timer�� ü�� �ڿ������� �����ϱ� ����
	float tempHealth = health;
	float addHealth  = health + InAmount;

	// #3. MaxHealth�� �Ѿ�� �����ǵ���
	addHealth = FMath::Clamp(addHealth, 0.f, MaxHealth);

	// #4. Timer ���ప + ���� ���� ��
	healthAmount += (addHealth - tempHealth);

	// #5. �ִ�ü���� �ƴ϶�� Timer ����
	if (healthAmount > 0.f)
		GetWorld()->GetTimerManager().UnPauseTimer(IncreaseHealthTimer);
}

void UCStatusComponent::SubHealth(const float& InAmount)
{
	// #1. ���� ����ǰ� �ִ� Ÿ�̸Ӹ� ����
	GetWorld()->GetTimerManager().PauseTimer(DecreaseHealthTimer);

	// #2. ���� ü�� ��� ���� -> Timer�� ü�� �ڿ������� �����ϱ� ����
	float tempHealth = health;
	float subHealth  = health - InAmount;

	// #3. 0.f���� �������� 0���� ����
	subHealth = FMath::Clamp(subHealth, 0.f, MaxHealth);

	// #4. Timer ���ప + ���� ���� ��
	healthAmount += (tempHealth - subHealth);

	// #5. �ִ�ü���� �ƴ϶�� Timer ����
	if (healthAmount > 0.f)
		GetWorld()->GetTimerManager().UnPauseTimer(DecreaseHealthTimer);
}

void UCStatusComponent::IncreaseHealth()
{
	// #1. ������ �Դ� ���� ������ healthAmount ���� �� IncreseTimer ����
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
	// #1. Clamp������ �ʿ���µ� ���� �ִ� ������ Tick���� 
	//     �� �Լ��� �����Ͽ� Clamp�� Tick���� �����Ű�� �ʱ� ����
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



