#include "Components/CSkillComponent.h"
#include "Global.h"

#include "CPlayerController.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_MainUI.h"
#include "Widgets/CUserWidget_SkillWindow.h"

#include "Action/CActionDataAsset.h"
#include "GameFramework/Character.h"

#include "Action/CWeapon.h"

#include "Components/CWeaponStateComponent.h"
#include "CStateComponent.h"
#include "Action/DoSkill/CDoSkill.h"

#include "Managers/UIManager.h"

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
    //WeaponState = Cast<UCWeaponStateComponent>(OwnerCharacter->GetComponentByClass(UCWeaponStateComponent::StaticClass()));


    ///////////////////////////////////////////////////////////////////////////////


    // #1. 클래스 디폴트에서 EWeaponState에 맞는 SkillWeaponData를 넣도록 설정하게 만들고
    //     넣은 것에 대해서만 AcionDataAsset클래스에서 Weapon 생성
    if (!!OwnerCharacter)
    {
        for (TMap<EWeaponStateType, UCActionDataAsset*>::TIterator It(SkillWeaponDatas); It; ++It)
        {
            if (It.Value() != nullptr)
            {
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

// #1. UIManger로부터 다른 UI가 한개라도 켜져있는지 확인하고 바꾸기
void UCSkillComponent::OpenUI()
{
    if (UIManager::CheckUIMode() == false)
        UIManager::SetUIMode();

    bIsVisible = true;
    MainUI->SetSkillWindowVisibility(bIsVisible);
}

// #1. UIManager를 통하여 ESC키를 눌렀을 때 실행되는 함수
void UCSkillComponent::ClearUI()
{
    //  #2. GameMode로 변경 및 마우스커서 설정 안하는 이유는 
    //      UIManager에서 딱 한번만 실행하도록 하기 위해서

    if (bIsVisible)
    {
        bIsVisible = false;

        UpdateSkillSlotDatas(MainUI->UpdateMainSlotSkills());
        MainUI->SetSkillWindowVisibility(bIsVisible);
    }
}

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

void UCSkillComponent::ActivateSkill(const int32& slotNum)
{
    FALSE_RETURN(State->IsIdleMode());
    // #1. 쿨타임 체크

    // #2. 스킬 사용할 무기가 있는지 확인해서 Data 가져오기
    if (slotNum >= 0 && slotNum < SkillSlotDatas.Num())
    {
        // SkillSlotDatas의 num번째 요소를 가져와서 해당 키 값을 추출

        const TMap<EWeaponStateType, int32>& SkillSlotData = SkillSlotDatas[slotNum];
        for (const auto& Pair : SkillSlotData)
        {
            EWeaponStateType WeaponType = Pair.Key;
            WeaponSkillIndex = Pair.Value;

            if (WeaponSkillIndex == -1)
                return;
            //SkillWeaponDatas.

            // 해당 키 값을 사용하여 SkillWeaponDatas에서 UCActionDataAsset*을 가져옴
            ACWeapon** FoundSkillWepaon = SkillWeapons.Find(WeaponType);

            if (FoundSkillWepaon != nullptr)
            {
                SkillWeaponNow = (*FoundSkillWepaon);
            }
        }
    }

    if (!!SkillWeaponNow)
    {
        // #3. 장착 중이면 잠깐 Cache(저장) 해놓고 스킬 사용후 돌려주기 
        //     장착 중이지 않으면 바로 스킬 발동 후 다시 Unarmed 상태로
        if (WeaponState->IsEquipped())
        {
            CacheWeapon = WeaponState->GetCurrentWeapon();
            WeaponState->UnequipWeapon();

            WeaponState->EquipWeapon(SkillWeaponNow);
            if (WeaponSkillIndex != -1)
                SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->Activate();
        }
        else
        {
            WeaponState->EquipWeapon(SkillWeaponNow);

            if (WeaponSkillIndex != -1)
                SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->Activate();
        }

        if (WeaponSkillIndex != -1)
            SkillWeaponNow->SetSkillWeaponVisibility(true);
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

void UCSkillComponent::OnAttackByTimer()
{
    CheckCanExecute();

    if (WeaponSkillIndex != -1)
    SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->OnAttackByTimer();
}

void UCSkillComponent::OffAttackByTimer()
{
    CheckCanExecute();

    if (WeaponSkillIndex != -1)
    SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->OffAttackByTimer();
}

void UCSkillComponent::DoSubSkill()
{
    CheckCanExecute();

    if (WeaponSkillIndex != -1)
    SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->DoSubSkill();
}

void UCSkillComponent::Begin_FromSlash()
{
    CheckCanExecute();

    if (WeaponSkillIndex != -1)
    SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->Begin_FromSlash();
}

void UCSkillComponent::End_FromSlash()
{
    CheckCanExecute();

    if (WeaponSkillIndex != -1)
    SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->End_FromSlash();
}

void UCSkillComponent::End_DoSkill()
{
    CheckCanExecute();

    if (WeaponSkillIndex != -1)
    {
        SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->End_DoSkill();
        SkillWeaponNow->SetSkillWeaponVisibility(false);
    }

    if (CacheWeapon != nullptr)
    {

        WeaponState->UnequipWeapon();
        WeaponState->EquipWeapon(CacheWeapon);
        CacheWeapon->SetSkillWeaponVisibility(true);
    }
    else
        WeaponState->UnequipWeapon();

    SkillWeaponNow = nullptr;
    CacheWeapon = nullptr;
    WeaponSkillIndex = -1;
}

void UCSkillComponent::End_Hold()
{
    CheckCanExecute();

    if (WeaponSkillIndex != -1)
    SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->End_Hold();
}