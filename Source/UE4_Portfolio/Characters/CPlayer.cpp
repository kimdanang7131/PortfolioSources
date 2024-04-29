#include "Characters/CPlayer.h"
#include "Global.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimInstance.h"

#include "Components/CInventoryComponent.h"
#include "Components/CSkillComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"
#include "Characters/CCombatantCharacter.h"
#include "Components/CWeaponStateComponent.h"

#include "Managers/UIManager.h"


// Test용
#include "CPlayerController.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_MainUI.h"
#include "Widgets/CUserWidget_InvenWindow.h"

#include "Widgets/CUserWidget_Stamina.h"
#include "Widgets/CUserWidget_Health.h"

#include "Actors/CItem.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	///
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetRootComponent());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	///
	CHelpers::CreateActorComponent<UCTargetComponent>(this, &Target, "Target");


	// 기본 SpringArm & Camera 셋팅
	{
		SpringArm->SetRelativeLocation(FVector(0, 0, 40));
		SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
		SpringArm->TargetArmLength = 500.f;
		SpringArm->SocketOffset = FVector(0, 0, 60.f);
		SpringArm->bDoCollisionTest = false;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bEnableCameraLag = true;
	}

}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();


	TArray<AActor*> TradersArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACTrader::StaticClass(), TradersArr);

	for (AActor* trader : TradersArr)
	{
		ACTrader* castedTrader = Cast<ACTrader>(trader);

		if (!!castedTrader)
		{
			// OpenTradeWindow 함수의 시그니처와 델리게이트의 시그니처가 일치하도록 AddDynamic 사용
			castedTrader->TradeBeginDelegate.AddDynamic(this, &ACPlayer::OpenTradeWindow);
			castedTrader->TradeEndDelegate.AddDynamic(this, &ACPlayer::CloseTradeWindow);
		}
	}


	PController = Cast<ACPlayerController>(GetController());
	PController->GetPlayerMainUI()->SetPlayerInvenWindowBool();

	HealthWidget  = PController->GetPlayerMainUI()->GetHealthBar();
	StaminaWidget = PController->GetPlayerMainUI()->GetStaminaBar();
	Status->SetHealthWidget(HealthWidget);

	MaxStamina = Status->GetMaxStamina();

	//WeaponState->OnWeaponStateTypeChanged.AddDynamic(this, &ACPlayer::OnWeaponStateTypeChanged);
	State->SetIdleMode();
	WeaponState->SetUnarmedMode();

	// 나중에 지울거
	PotionItem = CHelpers::SpawnActorOnRuntime<ACItem>(PotionItemClass, this, GetActorTransform());
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bIsSprinting)
	{
		Status->SubStamina(STAMINA_USAGE * DeltaTime);
		StaminaWidget->Update(Status->GetStamina(), MaxStamina);

		if (Status->GetStamina() <= 0.1f)
			OffSprint();
	}
	else
	{
		Status->AddStamina(STAMINA_RECOVERAGE * DeltaTime);
		StaminaWidget->Update(Status->GetStamina(), MaxStamina);
	}
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveFB", this, &ACPlayer::OnMoveFB);
	PlayerInputComponent->BindAxis("MoveLR", this, &ACPlayer::OnMoveLR);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);

	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed , this, &ACPlayer::OnSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ACPlayer::OffSprint);

	PlayerInputComponent->BindAction("Dodge", EInputEvent::IE_Pressed, this, &ACPlayer::ExecuteDodge);
	PlayerInputComponent->BindAction("ActionL", EInputEvent::IE_Pressed, this, &ACPlayer::DoActionL);
	//PlayerInputComponent->BindAction("ActionR", EInputEvent::IE_Pressed, this, &ACPlayer::OffSprint);
	//PlayerInputComponent->BindAction("ActionM", EInputEvent::IE_Pressed, this, &ACPlayer::OffSprint);

	PlayerInputComponent->BindAction("SkillZ", EInputEvent::IE_Pressed, this, &ACPlayer::SkillZ);
	PlayerInputComponent->BindAction("SkillZ", EInputEvent::IE_Released, this, &ACPlayer::End_Hold);

	PlayerInputComponent->BindAction("SkillX", EInputEvent::IE_Pressed, this, &ACPlayer::SkillX);
	PlayerInputComponent->BindAction("SkillX", EInputEvent::IE_Released, this, &ACPlayer::End_Hold);

	PlayerInputComponent->BindAction("SkillC", EInputEvent::IE_Pressed, this, &ACPlayer::SkillC);
	PlayerInputComponent->BindAction("SkillC", EInputEvent::IE_Released, this, &ACPlayer::End_Hold);

	PlayerInputComponent->BindAction("SkillV", EInputEvent::IE_Pressed, this, &ACPlayer::SkillV);
	PlayerInputComponent->BindAction("SkillV", EInputEvent::IE_Released, this, &ACPlayer::End_Hold);


	PlayerInputComponent->BindAction("WeaponA", EInputEvent::IE_Pressed, this, &ACPlayer::ToggleWeaponA);
	PlayerInputComponent->BindAction("WeaponB", EInputEvent::IE_Pressed, this, &ACPlayer::ToggleWeaponB);

	PlayerInputComponent->BindAction("ToggleSkillWindow", EInputEvent::IE_Pressed, this, &ACPlayer::ToggleSkillWindow);
	PlayerInputComponent->BindAction("ToggleInventoryWindow", EInputEvent::IE_Pressed, this, &ACPlayer::ToggleInventoryWindow);

	PlayerInputComponent->BindAction("Close", EInputEvent::IE_Pressed, this, &ACPlayer::ClearUI);

	PlayerInputComponent->BindAction("Test1", EInputEvent::IE_Pressed, this, &ACPlayer::Test1);
	PlayerInputComponent->BindAction("Test2", EInputEvent::IE_Pressed, this, &ACPlayer::Test2);

	PlayerInputComponent->BindAction("UseItemA", EInputEvent::IE_Pressed, this, &ACPlayer::UseItemA);
	PlayerInputComponent->BindAction("UseItemB", EInputEvent::IE_Pressed, this, &ACPlayer::UseItemB);
	PlayerInputComponent->BindAction("UseItemC", EInputEvent::IE_Pressed, this, &ACPlayer::UseItemC);
	PlayerInputComponent->BindAction("UseItemD", EInputEvent::IE_Pressed, this, &ACPlayer::UseItemD);
}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamId);
}

void ACPlayer::OnMoveFB(float InAxis)
{
	FALSE_RETURN(Status->CanMove())

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveLR(float InAxis)
{
	FALSE_RETURN(Status->CanMove())

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	FALSE_RETURN(Status->CanControl())
	float rate = Status->GetHorizontalLookRate();

	AddControllerYawInput(rate * InAxis * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	FALSE_RETURN(Status->CanControl())
	float rate = Status->GetVerticalLookRate();

	AddControllerPitchInput(rate * InAxis * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnSprint()
{
	FALSE_RETURN(Status->CanControl());

	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
}
void ACPlayer::OffSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void ACPlayer::DoActionL()
{
	WeaponState->DoAction();
}

void ACPlayer::ToggleWeaponA()
{
	WeaponState->ToggleWeaponA();
}

void ACPlayer::ToggleWeaponB()
{
	WeaponState->ToggleWeaponB();
}

void ACPlayer::ExecuteDodge()
{
	Super::ExecuteDodge();
}

void ACPlayer::SkillZ()
{
	Skill->SkillZ();
}

void ACPlayer::SkillX()
{
	Skill->SkillX();
}

void ACPlayer::SkillC()
{
	Skill->SkillC();
}

void ACPlayer::SkillV()
{
	Skill->SkillV();
}

void ACPlayer::End_Hold()
{
	Skill->End_Hold();
}

void ACPlayer::ToggleSkillWindow()
{
	Skill->ToggleSkillWindow();
}

void ACPlayer::ToggleInventoryWindow()
{
	Inventory->ToggleInventoryWindow();
}

void ACPlayer::ClearUI()
{
	UIManager::SetGameModeOnly();
}



void ACPlayer::OpenTradeWindow(const TArray<FItemDataTableBase>& InTraderFItems, const int32& InMoney)
{
	Inventory->OpenTraderWindow(InTraderFItems , InMoney);
}

void ACPlayer::CloseTradeWindow(ACTrader* InTrader)
{
	// 서순 제발..
	int32 tempMoney                      = Inventory->GetTraderMoney();
	TArray<FItemDataTableBase> tempInven = Inventory->GetTraderInvenDatas();
	InTrader->UpdateTraderInvenDatas(tempInven, tempMoney);

	// 위에꺼 다 옮기고 지워야함 2시간 날린듯
	Inventory->CloseTraderWindow();
}


void ACPlayer::Test1()
{
	Inventory->AddFItemToUI();
	//Inventory->TestPrint();
}

void ACPlayer::Test2()
{
	//Inventory->AddFItemToUI2();

	CLog::Print("Test2");

	//PotionItem->AcivateItem(FItem);

	//ACPlayerController* controller = Cast<ACPlayerController>(GetController());

	//if (!!controller)
	//{
	//	controller->GetPlayerMainUI()->GetInvenWindow()->TestAdd(0);
	//}
}

void ACPlayer::UseItemA()
{

}

void ACPlayer::UseItemB()
{

}

void ACPlayer::UseItemC()
{

}

void ACPlayer::UseItemD()
{

}
