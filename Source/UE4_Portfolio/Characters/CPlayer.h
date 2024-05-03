#pragma once

#include "CoreMinimal.h"
#include "Characters/CCombatantCharacter.h"

#include "Structs/CFItemDataTableBase.h"
#include "GenericTeamAgentInterface.h"
#include "Characters/CTrader.h"

#include "CPlayer.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACPlayer : public ACCombatantCharacter , public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
private:
	const float STAMINA_USAGE      = 10.f;
	const float STAMINA_RECOVERAGE = 15.f;

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Trader �����ȿ� ������ �� Delegate�� ���Ͽ� ������ ���� ������ȯ*/
private:
	/** Trader������ ������ Delegate�� ���� TraderInventory ���� */
	UFUNCTION()
		void OpenTradeWindow(const TArray<FItemDataTableBase>& InTraderFItems, const int32& InMoney);

	/** Trader������ ������ Delegate�� ���� Trader�� Inventory ��,������ ������Ʈ */
	UFUNCTION()
		void CloseTradeWindow(ACTrader* InTrader);

private:
	UPROPERTY(EditDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly)
		class UCameraComponent* Camera;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCTargetComponent* Target;

protected:
	/** �������̽��� ���� �� ���س��� */
	virtual FGenericTeamId GetGenericTeamId() const override;

	UPROPERTY(EditDefaultsOnly)
		uint8 TeamId = 0;



private:
	/** �յ��¿� ������ */
	void OnMoveFB(float InAxis);
	void OnMoveLR(float InAxis);

	/** �����¿� ���콺 ������*/
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);

	/** Shift �޸��� On Off */
	void OnSprint();
	void OffSprint();

	/** ��Ŭ�� �Ϲ� ���� , �޺� ���� */
	void DoActionL();

	/** Q , E ���� ���� ����*/
	void ToggleWeaponA() override;
	void ToggleWeaponB() override;

	/** ZXCV ���ν�ųâ UI�� ���� �� ��밡�� , Ȧ�� ���� */
	virtual void SkillZ() override;
	virtual void SkillX() override;
	virtual void SkillC() override;
	virtual void SkillV() override;
	void End_Hold();

	/** K �������� ��ųâ UI ����
	    I �������� �κ��丮â UI ���� */
	void ToggleSkillWindow();
	void ToggleInventoryWindow();

	/** ���� 5�� �������� UIManager�� ���Ͽ� ��� UI ���� */
	void ClearUI();

	/** 0123 �Է� �� ���� ���� ������â UI �ִٸ� ��� */
	void UseItemA();
	void UseItemB();
	void UseItemC();
	void UseItemD();

	/** DataAsset State�� ��ϵ� Animation�� ���� */
protected:
	virtual void Hitted() { Super::Hitted(); }
	virtual void Dead() { Super::Dead(); }
	virtual void Dodging() { Super::Dodging(); }


	// 9 , 10 �� �������� �׽�Ʈ ���� 9�� ���ǳֱ�
	void Test1();
	void Test2();

private:
	class ACPlayerController* MyController;


	/** Sprinting���� �� Stamina õõ�� ���̱� , ȸ��*/
	class UCUserWidget_Health*  HealthWidget;
	class UCUserWidget_Stamina* StaminaWidget;
	float MaxStamina = 0.f;
	bool bIsSprinting = false;


    /** ������ -> Test1,Test2�Լ��� ���� ����� */
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class ACItem> PotionItemClass;

private:
	class ACItem* PotionItem;
};
