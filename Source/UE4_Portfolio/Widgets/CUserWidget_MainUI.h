#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/CEweaponStateType.h"
#include "CUserWidget_MainUI.generated.h"


UCLASS()
class UE4_PORTFOLIO_API UCUserWidget_MainUI : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

public:
    FORCEINLINE class UCUserWidget_SkillWindow* GetSkillWindow()  { return SkillWindow; }
    FORCEINLINE class UCUserWidget_InvenWindow* GetInvenWindow()  { return InvenWindow; }
    FORCEINLINE class UCUserWidget_InvenWindow* GetTraderWindow() { return TraderWindow; }
    FORCEINLINE class UCUserWidget_Health*  GetHealthBar()   { return HealthBar; }
    FORCEINLINE class UCUserWidget_Stamina* GetStaminaBar()  { return StaminaBar; }


    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
        class UCanvasPanel* Canvas;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        class UCUserWidget_InvenWindow* InvenWindow;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        class UCUserWidget_InvenWindow* TraderWindow;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        class UCUserWidget_SkillWindow* SkillWindow;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        class UCUserWidget_Health* HealthBar;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        class UCUserWidget_Stamina* StaminaBar;

public:
    UPROPERTY(BlueprintReadWrite, Category = "MainUI")
        TArray<UCUserWidget_MainSkillSlot*> SkillMainSlots;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        class UCUserWidget_MainSkillSlot* SkillMainSlot_0;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        class UCUserWidget_MainSkillSlot* SkillMainSlot_1;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        class UCUserWidget_MainSkillSlot* SkillMainSlot_2;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        class UCUserWidget_MainSkillSlot* SkillMainSlot_3;

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        class UCUserWidget_InvenSlot* InvenMainSlot_0;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        class UCUserWidget_InvenSlot* InvenMainSlot_1;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        class UCUserWidget_InvenSlot* InvenMainSlot_2;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        class UCUserWidget_InvenSlot* InvenMainSlot_3;

public:
	UFUNCTION(BlueprintCallable, Category = "MainUI")
        class UCUserWidget_MainSkillSlot* FindIndexSkillMainSlot(const int32& InUseSkillIndex);

    UFUNCTION(BlueprintCallable, Category = "MainUI")
        int32 CheckSameSkillMainSlot(UTexture2D* InTexture);

    UFUNCTION(BlueprintCallable, Category = "MainUI")
        void SkillSlotSwap(const int32& InSwapIndex , const int32& InSwappedIndex);

    UFUNCTION(BlueprintCallable, Category = "MainUI")
        void SkillSlotClear(class UCUserWidget_MainSkillSlot* InMainSkillSlot);


    UFUNCTION(BlueprintImplementableEvent, Category = "MainUI | SkillWindow")
        void SetSkillWindowVisibility(bool IsVislbie);

    UFUNCTION(BlueprintImplementableEvent, Category = "MainUI | InvenWindow")
        void SetPlayerInventoryVisibility(bool IsVislbie);

    UFUNCTION(BlueprintImplementableEvent, Category = "MainUI | InvenWindow")
        void SetTraderInventoryVisibility(bool IsVislbie);

    UFUNCTION(BlueprintImplementableEvent, Category = "MainUI | InvenWindow")
        void SetPlayerInvenWindowBool();

    UFUNCTION(BlueprintCallable, Category = "MainUI | MainSkill")
        void SetMainSkillActivated(UTexture2D* InTexture);

    UFUNCTION(BlueprintCallable, Category = "MainUI")
        bool CheckCanTrade() { return this->bCanTrade; }

    UFUNCTION(BlueprintCallable, Category = "MainUI")
        void SetCanTrade(bool InbCanTrade) { this->bCanTrade = InbCanTrade; }

public:
    TArray<TMap<EWeaponStateType, int32>> UpdateMainSlotSkills();
    
    //void SetInventoryWindowVisibility(bool IsVislbie);
    //void SetTradeWindowVisibility(bool IsVislbie);


public:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
        class ACharacter* OwnerCharacter;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
        class UCInventoryComponent* InvenComp;

private:
    void Init();


private:
    bool bCanTrade = false;
};
