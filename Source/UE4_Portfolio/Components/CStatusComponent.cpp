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

	// #1. 기본 셋팅
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter != nullptr)
	{
		State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
		LockController = OwnerCharacter->GetController();
	}

	health  = MaxHealth;
	stamina = MaxStamina;

	// #2. Health관련 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(IncreaseHealthTimer, this, &UCStatusComponent::IncreaseHealth, 0.01f, true);
	GetWorld()->GetTimerManager().PauseTimer(IncreaseHealthTimer);

	GetWorld()->GetTimerManager().SetTimer(DecreaseHealthTimer, this, &UCStatusComponent::DecreaseHealth, 0.01f, true);
	GetWorld()->GetTimerManager().PauseTimer(DecreaseHealthTimer);
}

void UCStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/** HealthWidget을 따로 변수로 만들어 실시간으로 접근하여 UI 변경 */
void UCStatusComponent::SetHealthWidget(class UCUserWidget_Health* InHealthWidget)
{
	if (InHealthWidget == nullptr)
		return;

	HealthWidget = InHealthWidget;
	HealthWidget->Update(health, MaxHealth);
}

/** Hitted 했을 때 잠시 0으로 바꾸기 위해 */
void UCStatusComponent::SetMovementSpeed(const float& InSpeed)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = InSpeed;
}


/** 타이머를 통한 체력 증가 */
void UCStatusComponent::AddHealth(const float& InAmount)
{
	// #1. 현재 진행되고 있는 타이머를 정지
	GetWorld()->GetTimerManager().PauseTimer(IncreaseHealthTimer);

	healthAmount += InAmount;
	healthAmount = FMath::Clamp(healthAmount, 0.f, MaxHealth);

	// #2. 최대체력이 아니라면 Timer 진행
	if (healthAmount > 0.f)
		GetWorld()->GetTimerManager().UnPauseTimer(IncreaseHealthTimer);
}

/** 타이머를 통한 체력 감소 */
void UCStatusComponent::SubHealth(const float& InAmount)
{
	// #1. 현재 진행되고 있는 타이머를 정지
	GetWorld()->GetTimerManager().PauseTimer(DecreaseHealthTimer);

	healthAmount += InAmount;
	healthAmount = FMath::Clamp(healthAmount, 0.f, MaxHealth);

	// #2. 현재 빠지고 있는 체력 ( Timer를 통한 UI update ) 과  앞으로 빠질 체력을 종합해서
	//     이미 죽을 상태이면 DeadMode로 진행 , Sub 타이머는 그대로 진행
	if (health - healthAmount <= 0.f)
		State->SetDeadMode();

	// #2. 최대체력이 아니라면 Timer 진행
	if (healthAmount > 0.f)
		GetWorld()->GetTimerManager().UnPauseTimer(DecreaseHealthTimer);
}


/** 타이머 체력 증가 로직 */
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

/** 타이머 체력 감소 로직 */
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

/** 현재 Player만 공격시 Stamina 달도록 설정 */
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
	// #1. CanMove와는 반대의 값을 가짐 ( Ignore->true == bCanMove->false )
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
	// #1. CanControl과는 반대의 값을 가짐
	bCanControl = !bIgnoreLook;

	if (LockController)
		bIgnoreLook == true ? LockController->SetIgnoreLookInput(true) : LockController->ResetIgnoreLookInput();
}


