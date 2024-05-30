#pragma once


#include "CoreMinimal.h"
#include "Components/UIComponents/CUIManagementComponent.h"
#include "CPlayerController.h"
#include "Global.h"

class UE4_PORTFOLIO_API UIManager
{
public:
	/** PlayerController 등록 */
	static void SetPlayerController(class ACPlayerController* InPlayerController)
	{
		ACPlayerController* controller = Cast<ACPlayerController>(InPlayerController);

		if (!!controller)
		{
			PlayerController = controller;
		}
	}

	/** 시작할때 UIComponent의 상위부모의 BeginPlay에서 이 클래스의 AddUIComponent를 실행하여 자동등록 */
	static void AddUIComponent(class UCUIManagementComponent* UIComponent)
	{
		UIComponents.Add(UIComponent);
	}

	/** UIManager를 통해 모든 켜져있는 UI를 끄고 GameMode로 전환 */
	static void SetGameModeOnly()
	{
		if (UIComponents.Num() > 0)
		{
			for (int32 i = 0; i < UIComponents.Num(); i++)
			{
				UIComponents[i]->ClearUI();
			}

			if (!!PlayerController)
			{
				PlayerController->SetInputMode(FInputModeGameOnly());
				PlayerController->SetShowMouseCursor(false);
			}
		}
	}

	/** Input모드 변경 */
	static void SetUIMode()
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->SetShowMouseCursor(true);
	}

	/** UI가 한개라도 켜져있는지 확인 */
	static bool CheckUIMode()
	{
		if (UIComponents.Num() > 0)
		{
			for (int32 i = 0; i < UIComponents.Num(); i++)
			{
				if (UIComponents[i]->CheckUIMode())
					return true;
			}
		}

		return false;
	}

public:
	static TArray<class UCUIManagementComponent*> UIComponents;
	static class ACPlayerController* PlayerController;
};