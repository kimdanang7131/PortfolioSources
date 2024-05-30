#include "CPlayerController.h"
#include "Global.h"
////////////////////////
#include "Widgets/CUserWidget_MainUI.h"
#include "Characters/CPlayer.h" 
////////////////////////
#include "Components/WidgetComponent.h"

class UCUserWidget_MainUI* ACPlayerController::MainUI = nullptr;

ACPlayerController::ACPlayerController()
{

}

/** PlayerController ������ MainUI Viewport�� ���� */
void ACPlayerController::BeginPlay()
{
    Super::BeginPlay();
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