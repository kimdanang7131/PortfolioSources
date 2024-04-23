
#include "Components/CUIManagementComponent.h"

#include "Global.h"

#include "GameFramework/Character.h"

#include "CPlayerController.h"
#include "Widgets/CUserWidget_MainUI.h"

#include "Managers/UIManager.h"


// Sets default values for this component's properties
UCUIManagementComponent::UCUIManagementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UCUIManagementComponent::BeginPlay()
{
	Super::BeginPlay();

    OwnerCharacter = Cast<ACharacter>(GetOwner());
    PlayerController = Cast<ACPlayerController>(OwnerCharacter->GetController());


    // MainUI µî·Ï
    if (!!PlayerController)
    {
        MainUI = PlayerController->GetPlayerMainUI();
        UIManager::SetPlayerController(PlayerController);

        if (OwnerCharacter == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
            UIManager::AddUIComponent(this);
    }
}

