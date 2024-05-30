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

/** PlayerController 생성시 MainUI Viewport에 생성 */
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
//	// 플레이어 캐릭터를 가져오기 위해 GetWorld()->GetFirstPlayerController()를 사용할 수 있습니다.
//	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
//	if (PlayerController)
//	{
//		// ACPlayer로 캐스팅하여 ACPlayer에 대한 포인터를 반환합니다.
//		return Cast<ACPlayer>(PlayerController->GetPawn());
//	}
//
//	return nullptr;
//}