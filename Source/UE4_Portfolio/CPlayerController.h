#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

	ACPlayerController();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintPure)
		static class UCUserWidget_MainUI* GetPlayerMainUI();

	//UFUNCTION(BlueprintPure, Category = "Player")
	//	static class ACPlayer* GetPlayer();

protected:
	// MainUI에 접근하는 변수
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class UCUserWidget_MainUI> MainUIWidgetClass;

private:
	static class UCUserWidget_MainUI* MainUI; // 정적으로 변경
};
