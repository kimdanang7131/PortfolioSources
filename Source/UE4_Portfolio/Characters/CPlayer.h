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

	/** Trader 영역안에 들어왔을 때 Delegate를 통하여 아이템 관련 정보교환*/
private:
	/** Trader영역에 들어오면 Delegate를 통해 TraderInventory 오픈 */
	UFUNCTION()
		void OpenTradeWindow(const TArray<FItemDataTableBase>& InTraderFItems, const int32& InMoney);

	/** Trader영역을 나가면 Delegate를 통해 Trader의 Inventory 돈,아이템 업데이트 */
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
	/** 인터페이스를 통한 팀 정해놓기 */
	virtual FGenericTeamId GetGenericTeamId() const override;

	UPROPERTY(EditDefaultsOnly)
		uint8 TeamId = 0;



private:
	/** 앞뒤좌우 움직임 */
	void OnMoveFB(float InAxis);
	void OnMoveLR(float InAxis);

	/** 상하좌우 마우스 움직임*/
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);

	/** Shift 달리기 On Off */
	void OnSprint();
	void OffSprint();

	/** 좌클릭 일반 공격 , 콤보 연계 */
	void DoActionL();

	/** Q , E 무기 장착 해제*/
	void ToggleWeaponA() override;
	void ToggleWeaponB() override;

	/** ZXCV 메인스킬창 UI가 있을 때 사용가능 , 홀드 해제 */
	virtual void SkillZ() override;
	virtual void SkillX() override;
	virtual void SkillC() override;
	virtual void SkillV() override;
	void End_Hold();

	/** K 눌렀을때 스킬창 UI 오픈
	    I 눌렀을때 인벤토리창 UI 오픈 */
	void ToggleSkillWindow();
	void ToggleInventoryWindow();

	/** 현재 5번 눌렀을때 UIManager를 통하여 모든 UI 끄기 */
	void ClearUI();

	/** 0123 입력 시 통해 메인 아이템창 UI 있다면 사용 */
	void UseItemA();
	void UseItemB();
	void UseItemC();
	void UseItemD();

	/** DataAsset State에 등록된 Animation을 실행 */
protected:
	virtual void Hitted() { Super::Hitted(); }
	virtual void Dead() { Super::Dead(); }
	virtual void Dodging() { Super::Dodging(); }


	// 9 , 10 번 눌렀을때 테스트 현재 9번 포션넣기
	void Test1();
	void Test2();

private:
	class ACPlayerController* MyController;


	/** Sprinting했을 때 Stamina 천천히 줄이기 , 회복*/
	class UCUserWidget_Health*  HealthWidget;
	class UCUserWidget_Stamina* StaminaWidget;
	float MaxStamina = 0.f;
	bool bIsSprinting = false;


    /** 디버깅용 -> Test1,Test2함수와 같이 지우기 */
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class ACItem> PotionItemClass;

private:
	class ACItem* PotionItem;
};
