#include "CGameMode.h"
#include "Global.h"
#include "Managers/UIManager.h"

ACGameMode::ACGameMode()
{
    CHelpers::MyFClassFinder<APawn>(&DefaultPawnClass, "Blueprint'/Game/Blueprints/Characters/BP_CPlayer.BP_CPlayer_C'");
    CHelpers::MyFClassFinder<APlayerController>(&PlayerControllerClass, "Blueprint'/Game/Blueprints/BP_CPlayerController.BP_CPlayerController_C'");

    UIManager::UIComponents.Empty();
}