#pragma once


#include "CoreMinimal.h"
#include "Components/CUIManagementComponent.h"
#include "CPlayerController.h"
#include "Global.h"

class UE4_PORTFOLIO_API UIManager
{
public:


	static void SetPlayerController(class ACPlayerController* InPlayerController)
	{
		ACPlayerController* controller = Cast<ACPlayerController>(InPlayerController);

		if (!!controller)
		{
			PlayerController = controller;
		}
	}

	// #1. �����Ҷ� UIComponent�� �����θ��� BeginPlay���� �� Ŭ������ AddUIComponent�� �����Ͽ� �ڵ����
	static void AddUIComponent(class UCUIManagementComponent* UIComponent)
	{
		UIComponents.Add(UIComponent);
		CLog::Print(UIComponents.Num());
	}

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

	static void SetUIMode()
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->SetShowMouseCursor(true);
	}

	// #1. UI�� �Ѱ��� �����ִ��� Ȯ��
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