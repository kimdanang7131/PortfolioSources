#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CUIManagementComponent.generated.h"

/** UIComponent�� ����� ���� PlayerUI�� UIManager�� ���� �ڵ���� */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCUIManagementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCUIManagementComponent();
protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	FORCEINLINE bool CheckUIMode() { return bIsVisible; }

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class ACharacter* OwnerCharacter;

public:
	/** �ڽ� Component���� ������ */
	virtual void OpenUI()  { }
	virtual void ClearUI() { }

protected:
	class ACPlayerController* PlayerController;
	class UCUserWidget_MainUI* MainUI;

	bool bIsVisible = false;
};
