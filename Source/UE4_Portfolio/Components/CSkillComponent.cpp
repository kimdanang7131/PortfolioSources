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

    // #1. Map�� Key -> Enum / Value -> nullptr�� ��� ���� �������� �����ϱ� ������
    for (int32 i = 0; i < (int32)EWeaponStateType::Max; ++i)
    {
        SkillWeaponDatas.Add((EWeaponStateType)i, nullptr); 
        SkillWeapons.Emplace((EWeaponStateType)i, nullptr); // Emplace �Լ� ���
    }
}
 
void UCSkillComponent::BeginPlay()
{
	Super::BeginPlay();

    WeaponState = CHelpers::GetComponent<UCWeaponStateComponent>(OwnerCharacter);
    State       = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
    //WeaponState = Cast<UCWeaponStateComponent>(OwnerCharacter->GetComponentByClass(UCWeaponStateComponent::StaticClass()));


    ///////////////////////////////////////////////////////////////////////////////


    // #1. Ŭ���� ����Ʈ���� EWeaponState�� �´� SkillWeaponData�� �ֵ��� �����ϰ� �����
    //     ���� �Ϳ� ���ؼ��� AcionDataAssetŬ�������� Weapon ����
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

    // #2. SkillDataTable�� ����� ������ �ִٸ� MainUI���� ���̵��� Create
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
    // #1. ��ųâ�� ���� ���� ��
    if (bIsVisible)
    {
        ClearUI();

        // #2. �ٸ� UI�� ��� ������������ ���� ���� ����
        if (UIManager::CheckUIMode() == false)
            UIManager::SetGameModeOnly();
    }
    else
        OpenUI();
}

// #1. UIManger�κ��� �ٸ� UI�� �Ѱ��� �����ִ��� Ȯ���ϰ� �ٲٱ�
void UCSkillComponent::OpenUI()
{
    if (UIManager::CheckUIMode() == false)
        UIManager::SetUIMode();

    bIsVisible = true;
    MainUI->SetSkillWindowVisibility(bIsVisible);
}

// #1. UIManager�� ���Ͽ� ESCŰ�� ������ �� ����Ǵ� �Լ�
void UCSkillComponent::ClearUI()
{
    //  #2. GameMode�� ���� �� ���콺Ŀ�� ���� ���ϴ� ������ 
    //      UIManager���� �� �ѹ��� �����ϵ��� �ϱ� ���ؼ�

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
    // #1. ��Ÿ�� üũ

    // #2. ��ų ����� ���Ⱑ �ִ��� Ȯ���ؼ� Data ��������
    if (slotNum >= 0 && slotNum < SkillSlotDatas.Num())
    {
        // SkillSlotDatas�� num��° ��Ҹ� �����ͼ� �ش� Ű ���� ����

        const TMap<EWeaponStateType, int32>& SkillSlotData = SkillSlotDatas[slotNum];
        for (const auto& Pair : SkillSlotData)
        {
            EWeaponStateType WeaponType = Pair.Key;
            WeaponSkillIndex = Pair.Value;

            if (WeaponSkillIndex == -1)
                return;
            //SkillWeaponDatas.

            // �ش� Ű ���� ����Ͽ� SkillWeaponDatas���� UCActionDataAsset*�� ������
            ACWeapon** FoundSkillWepaon = SkillWeapons.Find(WeaponType);

            if (FoundSkillWepaon != nullptr)
            {
                SkillWeaponNow = (*FoundSkillWepaon);
            }
        }
    }

    if (!!SkillWeaponNow)
    {
        // #3. ���� ���̸� ��� Cache(����) �س��� ��ų ����� �����ֱ� 
        //     ���� ������ ������ �ٷ� ��ų �ߵ� �� �ٽ� Unarmed ���·�
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



// Unarmed ���� or ���� ���Ⱑ ��������
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