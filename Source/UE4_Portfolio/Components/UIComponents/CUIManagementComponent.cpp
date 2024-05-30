#include "Components/UIComponents/CUIManagementComponent.h"
#include "Global.h"
//////////////////////////////
#include "CPlayerController.h"
#include "Managers/UIManager.h"
#include "Widgets/CUserWidget_MainUI.h"
//////////////////////////////
#include "GameFramework/Character.h"

UCUIManagementComponent::UCUIManagementComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

/** �� ������Ʈ�� �����ϴ� Actor�� Player�϶��� Component �����Ǹ� UIManager�� ���Ͽ� �ڵ� ��� */
void UCUIManagementComponent::BeginPlay()
{
	Super::BeginPlay();

    OwnerCharacter   = Cast<ACharacter>(GetOwner());
    PlayerController = Cast<ACPlayerController>(OwnerCharacter->GetController());

    // #1. MainUI ���
    if (!!PlayerController)
    {
        MainUI = PlayerController->GetPlayerMainUI();
        UIManager::SetPlayerController(PlayerController);

        if (OwnerCharacter == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
            UIManager::AddUIComponent(this);
    }
}

void UCUIManagementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
