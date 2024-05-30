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

/** 이 컴포넌트를 소유하는 Actor가 Player일때만 Component 생성되면 UIManager를 통하여 자동 등록 */
void UCUIManagementComponent::BeginPlay()
{
	Super::BeginPlay();

    OwnerCharacter   = Cast<ACharacter>(GetOwner());
    PlayerController = Cast<ACPlayerController>(OwnerCharacter->GetController());

    // #1. MainUI 등록
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
