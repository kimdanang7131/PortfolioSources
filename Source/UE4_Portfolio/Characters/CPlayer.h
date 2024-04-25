#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CCombatantCharacter.h"
#include "Structs/CFItemDataTableBase.h"
#include "GenericTeamAgentInterface.h"
#include "Characters/CTrader.h"
#include "CPlayer.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACPlayer : public ACCombatantCharacter , public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
private:
	const float STAMINA_USAGE   = 10.f;
	const float STAMINA_RECOVERAGE = 15.f;


private:
	UPROPERTY(EditDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly)
		class UCameraComponent* Camera;

	//UPROPERTY(VisibleDefaultsOnly)
	//	class UWidgetComponent* HealthWidget;

	//UPROPERTY(VisibleDefaultsOnly)
	//	class UWidgetComponent* StaminaWidget;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCTargetComponent* Target;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	UPROPERTY(EditDefaultsOnly)
		uint8 TeamId = 0;

private:
	void OnMoveFB(float InAxis);
	void OnMoveLR(float InAxis);

	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);

	void OnSprint();
	void OffSprint();

	void DoActionL();

	void ToggleWeaponA() override;
	void ToggleWeaponB() override;

	void ToggleSkillWindow();
	void ToggleInventoryWindow();

	virtual void SkillZ() override;
	virtual void SkillX() override;
	virtual void SkillC() override;
	virtual void SkillV() override;

	void End_Hold();
	void ClearUI();

	void Test1();
	void Test2();


	void UseItemA();
	void UseItemB();
	void UseItemC();
	void UseItemD();

protected:
	virtual void Hitted() { Super::Hitted(); }
	virtual void Dead() { Super::Dead(); }
	virtual void Dodging() { Super::Dodging(); }

private:
	UFUNCTION()
		void OpenTradeWindow(const TArray<FItemDataTableBase>& InTraderFItems , const int32& InMoney);

	UFUNCTION()
		void CloseTradeWindow(ACTrader* InTrader);

private:
	class ACPlayerController* PController;

	class UCUserWidget_Health*  HealthWidget;
	class UCUserWidget_Stamina* StaminaWidget;

	
	float MaxStamina = 0.f;
	bool bIsSprinting = false;


	// 디버깅 후 없애기
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class ACItem> PotionItemClass;

private:
	class ACItem* PotionItem;
};
