#include "CPlayerController.h"

#include "Global.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_MainUI.h"

#include "Characters/CPlayer.h" 

class UCUserWidget_MainUI* ACPlayerController::MainUI = nullptr;

ACPlayerController::ACPlayerController()
{
    //btick
	//TSubclassOf<UCUserWidget_Name> nameClass;
	//CHelpers::GetClass<UCUserWidget_Name>(&nameClass, "WidgetBlueprint'/Game/Blueprints/Widgets/WBP_Name.WBP_Name_C'");
	//NameWidget->SetWidgetClass(nameClass);
	//NameWidget->SetRelativeLocation(FVector(0, 0, 240));
	//NameWidget->SetDrawSize(FVector2D(240, 30));
	//NameWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ACPlayerController::BeginPlay()
{
    Super::BeginPlay();
    // MainUI ���� �� ȭ�鿡 �߰�
    if (MainUIWidgetClass)
    {
        MainUI = CreateWidget<UCUserWidget_MainUI>(GetWorld(), MainUIWidgetClass);

        if (!!MainUI)
            MainUI->AddToViewport();
    }
}

void ACPlayerController::Tick(float DeltaTime)
{
}


UCUserWidget_MainUI* ACPlayerController::GetPlayerMainUI()
{
    return MainUI;
}

//ACPlayer* ACPlayerController::GetPlayer()
//{
//	// �÷��̾� ĳ���͸� �������� ���� GetWorld()->GetFirstPlayerController()�� ����� �� �ֽ��ϴ�.
//	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
//	if (PlayerController)
//	{
//		// ACPlayer�� ĳ�����Ͽ� ACPlayer�� ���� �����͸� ��ȯ�մϴ�.
//		return Cast<ACPlayer>(PlayerController->GetPawn());
//	}
//
//	return nullptr;
//}