#include "Components/CStatusComponent.h"
#include "Global.h"
///////////////////////////
#include "Components/CStateComponent.h"
///////////////////////////
#include "GameFramework/Character.h"
#include "Widgets/CUserWidget_MainUI.h"
#include "Widgets/CUserWidget_Health.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"
///////////////////////////
#include "CPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UCStatusComponent::UCStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// #1. �⺻ ����
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter != nullptr)
	{
		State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
		LockController = OwnerCharacter->GetController();
	}

	health  = MaxHealth;
	stamina = MaxStamina;

	// #2. Health���� Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(IncreaseHealthTimer, this, &UCStatusComponent::IncreaseHealth, 0.01f, true);
	GetWorld()->GetTimerManager().PauseTimer(IncreaseHealthTimer);

	GetWorld()->GetTimerManager().SetTimer(DecreaseHealthTimer, this, &UCStatusComponent::DecreaseHealth, 0.01f, true);
	GetWorld()->GetTimerManager().PauseTimer(DecreaseHealthTimer);
}

void UCStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/** HealthWidget�� ���� ������ ����� �ǽð����� �����Ͽ� UI ���� */
void UCStatusComponent::SetHealthWidget(class UCUserWidget_Health* InHealthWidget)
{
	if (InHealthWidget == nullptr)
		return;

	HealthWidget = InHealthWidget;
	HealthWidget->Update(health, MaxHealth);
}

/** Hitted ���� �� ��� 0���� �ٲٱ� ���� */
void UCStatusComponent::SetMovementSpeed(const float& InSpeed)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = InSpeed;
}


/** Ÿ�̸Ӹ� ���� ü�� ���� */
void UCStatusComponent::AddHealth(const float& InAmount)
{
	// #1. ���� ����ǰ� �ִ� Ÿ�̸Ӹ� ����
	GetWorld()->GetTimerManager().PauseTimer(IncreaseHealthTimer);

	healthAmount += InAmount;
	healthAmount = FMath::Clamp(healthAmount, 0.f, MaxHealth);

	// #2. �ִ�ü���� �ƴ϶�� Timer ����
	if (healthAmount > 0.f)
		GetWorld()->GetTimerManager().UnPauseTimer(IncreaseHealthTimer);
}

/** Ÿ�̸Ӹ� ���� ü�� ���� */
void UCStatusComponent::SubHealth(const float& InAmount)
{
	// #1. ���� ����ǰ� �ִ� Ÿ�̸Ӹ� ����
	GetWorld()->GetTimerManager().PauseTimer(DecreaseHealthTimer);

	healthAmount += InAmount;
	healthAmount = FMath::Clamp(healthAmount, 0.f, MaxHealth);

	// #2. ���� ������ �ִ� ü�� ( Timer�� ���� UI update ) ��  ������ ���� ü���� �����ؼ�
	//     �̹� ���� �����̸� DeadMode�� ���� , Sub Ÿ�̸Ӵ� �״�� ����
	if (health - healthAmount <= 0.f)
		State->SetDeadMode();

	// #2. �ִ�ü���� �ƴ϶�� Timer ����
	if (healthAmount > 0.f)
		GetWorld()->GetTimerManager().UnPauseTimer(DecreaseHealthTimer);
}


/** Ÿ�̸� ü�� ���� ���� */
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

/** Ÿ�̸� ü�� ���� ���� */
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

/** ���� Player�� ���ݽ� Stamina �޵��� ���� */
void UCStatusComponent::SubStamina(const float& InAmount)
{
	if (OwnerCharacter->IsA<ACPlayer>())
	{
		stamina -= InAmount;
		stamina = FMath::Clamp(stamina, 0.0f, MaxStamina);
	}
}


// Movement
void UCStatusComponent::SetMove()
{
	LockMoveInput(false);
}

void UCStatusComponent::SetStop()
{
	LockMoveInput(true);
}

void UCStatusComponent::LockMoveInput(const bool& bIgnoreInput)
{
	// #1. CanMove�ʹ� �ݴ��� ���� ���� ( Ignore->true == bCanMove->false )
	bCanMove = !bIgnoreInput;
	
	if (LockController)
		bIgnoreInput == true ? LockController->SetIgnoreMoveInput(true) : LockController->ResetIgnoreMoveInput();
}


// MouseMove
void UCStatusComponent::SetControl()
{
	LockMouseMove(false);
}

void UCStatusComponent::SetStopControl()
{
	LockMouseMove(true);
}

void UCStatusComponent::LockMouseMove(const bool& bIgnoreLook)
{
	// #1. CanControl���� �ݴ��� ���� ����
	bCanControl = !bIgnoreLook;

	if (LockController)
		bIgnoreLook == true ? LockController->SetIgnoreLookInput(true) : LockController->ResetIgnoreLookInput();
}


