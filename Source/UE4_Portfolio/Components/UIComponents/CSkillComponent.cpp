#include "Components/UIComponents/CSkillComponent.h"
#include "Global.h"
//////////////////////////
#include "Components/CWeaponStateComponent.h"
#include "Components/CStateComponent.h"
//////////////////////////
#include "CPlayerController.h"
#include "Widgets/CUserWidget_MainUI.h"
#include "Widgets/CUserWidget_SkillWindow.h"
#include "Action/CActionDataAsset.h"
#include "Action/CWeapon.h"
#include "Action/DoSkill/CDoSkill.h"
#include "Managers/UIManager.h"
//////////////////////////
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
//////////////////////////

UCSkillComponent::UCSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

    // #1. Map의 Key -> Enum / Value -> nullptr로 모두 만들어서 블프에서 설정하기 쉽도록
    for (int32 i = 0; i < (int32)EWeaponStateType::Max; ++i)
    {
        SkillWeaponDatas.Add((EWeaponStateType)i, nullptr); 
        SkillWeapons.Emplace((EWeaponStateType)i, nullptr); // Emplace 함수 사용
    }
}
 
void UCSkillComponent::BeginPlay()
{
	Super::BeginPlay();

    WeaponState = CHelpers::GetComponent<UCWeaponStateComponent>(OwnerCharacter);
    State       = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);

    ///////////////////////////////////////////////////////////////////////////////

    // #1. 클래스 디폴트에서 EWeaponState에 맞는 SkillWeaponData를 넣도록 설정하게 만들고
    //     넣은 것에 대해서만 AcionDataAsset클래스에서 Weapon 생성
    if (!!OwnerCharacter)
    {
        for (TMap<EWeaponStateType, UCActionDataAsset*>::TIterator It(SkillWeaponDatas); It; ++It)
        {
            if (It.Value() != nullptr)
            {
                // 현재 CustomBeginPlay ACWeapon* 반환 이거 이용해서 한던지 해야함
                It.Value()->CustomBeginPlay(OwnerCharacter, It.Key());
                It.Value()->GetWeapon()->SetSkillWeaponVisibility(false);
                SkillWeapons.Emplace(It.Key(), It.Value()->GetWeapon());
            }
        }
    }

    // #2. SkillDataTable을 등록해 놓은게 있다면 MainUI에서 보이도록 Create
    if (!!SkillDT)
    {
        if (!!MainUI)
            MainUI->GetSkillWindow()->CreateSkillSlots(SkillDT);
    }
}

void UCSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/** Player와의 상호작용을 통해 UI On/Off */
void UCSkillComponent::ToggleSkillWindow()
{
    // #1. 스킬창이 열려 있을 때
    if (bIsVisible)
    {
        ClearUI();

        // #2. 다른 UI가 모두 꺼져있을때만 게임 모드로 변경
        if (UIManager::CheckUIMode() == false)
            UIManager::SetGameModeOnly();
    }
    else
        OpenUI();
}

/** UIManger로부터 다른 UI가 한개라도 켜져있는지 확인하고 바꾸기 */
void UCSkillComponent::OpenUI()
{
    // #1. 다른 UI가 모두 꺼져있을때만 SetUIMode로 바꾸기
    if (UIManager::CheckUIMode() == false)
        UIManager::SetUIMode();

    bIsVisible = true;
    MainUI->SetSkillWindowVisibility(bIsVisible);
}

/** UIManager를 통하여 ESC키를 눌렀을 때 실행되는 함수 */
void UCSkillComponent::ClearUI()
{
    //  #1. GameMode로 변경 및 마우스커서 설정 안하는 이유는 
    //      UIManager에서 딱 한번만 실행하도록 하기 위해서

    if (bIsVisible)
    {
        bIsVisible = false;

        UpdateSkillSlotDatas(MainUI->UpdateMainSlotSkills());
        MainUI->SetSkillWindowVisibility(bIsVisible);
    }
}

/** Player에서 사용할 함수들 */
void UCSkillComponent::SkillZ()
{
    ActivateSkill(0);
}

void UCSkillComponent::SkillX()
{
    ActivateSkill(1);
}

void UCSkillComponent::SkillC()
{
    ActivateSkill(2);
}

void UCSkillComponent::SkillV()
{
    ActivateSkill(3);
}


/** Skill ZXCV -> 0123 매번 SKill창을 끌때마다 여기 변수에 Update되어서
    변수로 0123에 스킬이 있다면 스킬 사용                               */
void UCSkillComponent::ActivateSkill(const int32& slotNum)
{
    FALSE_RETURN(State->IsIdleMode());
    // #1. 쿨타임 체크

    // #2. 스킬 사용할 무기가 있는지 확인해서 DataAsset 가져오기
    if (slotNum >= 0 && slotNum < SkillSlotDatas.Num())
    {
        // #3. TArray에 TMap이 0 , 1 , 2 ,3 저장되어있어서 각 번호에 맞게 가져오기 -> Key와 Value 둘다 필요
        const TMap<EWeaponStateType, int32>& SkillSlotData = SkillSlotDatas[slotNum];
        for (const auto& Pair : SkillSlotData)
        {
            EWeaponStateType WeaponType = Pair.Key;
            WeaponSkillIndex            = Pair.Value;

            if (WeaponSkillIndex == -1)
                return;

            // #4. 찾은 키 값을 이용하여 SkillWeaponDatas에서 DataAsset*을 가져옴
            ACWeapon** FoundSkillWepaon = SkillWeapons.Find(WeaponType);

            if (*FoundSkillWepaon != nullptr)
            {
                SkillWeaponNow = (*FoundSkillWepaon);
                break;
            }
        }
    }

    // #5. 사용가능한 SkillWeaponData가 있다면 
    if (SkillWeaponNow != nullptr)
    {
        if (SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->CheckCanActivateSkill())
        {
            SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->SetMainSkillWidget(ACPlayerController::GetPlayerMainUI()->FindIndexSkillMainSlot(slotNum));
            // Weapon을 장착 중이면 잠깐 Cache(저장) 해놓고 스킬 사용후 돌려주기 
            if (WeaponState->IsEquipped())
            {
                CacheWeapon = WeaponState->GetCurrentWeapon();
                WeaponState->UnequipWeapon();

                // 스킬 Weapon으로 잠시 변경 후 스킬 사용
                WeaponState->EquipWeapon(SkillWeaponNow);
                if (WeaponSkillIndex != -1)
                    SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->Activate();
            }
            //  Weapon을 장착 중이지 않으면 바로 스킬 발동 후 다시 Unarmed 상태로
            else
            {
                // 스킬 Weapon으로 잠시 변경 후 스킬 사용
                WeaponState->EquipWeapon(SkillWeaponNow);
                if (WeaponSkillIndex != -1)
                    SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->Activate();
            }

            // #6. 스킬 Weapon 잠시 보이도록 설정
            if (WeaponSkillIndex != -1)
            {
                SkillWeaponNow->SetSkillWeaponVisibility(true);
            }
        }
        else
        {
            CLog::Print("Skill OnCool");
        }
    }
    else
    {
        CLog::Print("SkillComp SkillWeaponNow Null");
    }
}


// Unarmed 모드거 or 현재 무기가 있을때만
bool UCSkillComponent::CheckCanExecute()
{
    return (WeaponState->IsUnarmedMode() == false) && (WeaponState->GetCurrentWeapon() != nullptr);
}

void UCSkillComponent::UpdateSkillSlotDatas(const TArray<TMap<EWeaponStateType, int32>>& newSkillData)
{
    SkillSlotDatas.Empty();
    SkillSlotDatas = newSkillData;
}

void UCSkillComponent::Reset()
{
    End_DoSkill();
}

#pragma region Notify Interact

void UCSkillComponent::OnAttackByTimer()
{
    if (WeaponSkillIndex != -1)
    SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->OnAttackByTimer();
}

void UCSkillComponent::OffAttackByTimer()
{
    if (WeaponSkillIndex != -1)
    SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->OffAttackByTimer();
}

void UCSkillComponent::DoSubSkill()
{
    if (WeaponSkillIndex != -1)
    SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->DoSubSkill();
}

void UCSkillComponent::Begin_FromSlash()
{
    if (WeaponSkillIndex != -1)
    SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->Begin_FromSlash();
}

void UCSkillComponent::End_FromSlash()
{
    if (WeaponSkillIndex != -1)
    SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->End_FromSlash();
}

/** 스킬 사용 끝나면 CacheWeapon으로 다시 쥐어주기 */
void UCSkillComponent::End_DoSkill()
{
    //
    // Dissolve 들어가야함
    //

    // #1. 현재 스킬을 종료하고 Visibility를 꺼줌
    if (WeaponSkillIndex != -1)
    {
        SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->End_DoSkill();
        SkillWeaponNow->SetSkillWeaponVisibility(false);
        WeaponState->UnequipWeapon();
    }

    // #2. 스킬 사용전 저장한 들고있던 Weapon으로 교체하고 Weapon 다시 보이도록
    if (CacheWeapon != nullptr)
    {
        WeaponState->EquipWeapon(CacheWeapon);
        CacheWeapon->SetSkillWeaponVisibility(true);
    }

    // #3. 설정 초기화 
    SkillWeaponNow = nullptr;
    CacheWeapon    = nullptr;
    WeaponSkillIndex = -1;
}

void UCSkillComponent::End_Hold()
{
    if (WeaponSkillIndex != -1)
    {
        SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->End_Hold();
    }
}

#pragma endregion