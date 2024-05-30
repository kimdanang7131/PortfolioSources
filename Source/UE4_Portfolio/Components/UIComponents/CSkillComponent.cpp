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

    ///////////////////////////////////////////////////////////////////////////////

    // #1. Ŭ���� ����Ʈ���� EWeaponState�� �´� SkillWeaponData�� �ֵ��� �����ϰ� �����
    //     ���� �Ϳ� ���ؼ��� AcionDataAssetŬ�������� Weapon ����
    if (!!OwnerCharacter)
    {
        for (TMap<EWeaponStateType, UCActionDataAsset*>::TIterator It(SkillWeaponDatas); It; ++It)
        {
            if (It.Value() != nullptr)
            {
                // ���� CustomBeginPlay ACWeapon* ��ȯ �̰� �̿��ؼ� �Ѵ��� �ؾ���
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

/** Player���� ��ȣ�ۿ��� ���� UI On/Off */
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

/** UIManger�κ��� �ٸ� UI�� �Ѱ��� �����ִ��� Ȯ���ϰ� �ٲٱ� */
void UCSkillComponent::OpenUI()
{
    // #1. �ٸ� UI�� ��� ������������ SetUIMode�� �ٲٱ�
    if (UIManager::CheckUIMode() == false)
        UIManager::SetUIMode();

    bIsVisible = true;
    MainUI->SetSkillWindowVisibility(bIsVisible);
}

/** UIManager�� ���Ͽ� ESCŰ�� ������ �� ����Ǵ� �Լ� */
void UCSkillComponent::ClearUI()
{
    //  #1. GameMode�� ���� �� ���콺Ŀ�� ���� ���ϴ� ������ 
    //      UIManager���� �� �ѹ��� �����ϵ��� �ϱ� ���ؼ�

    if (bIsVisible)
    {
        bIsVisible = false;

        UpdateSkillSlotDatas(MainUI->UpdateMainSlotSkills());
        MainUI->SetSkillWindowVisibility(bIsVisible);
    }
}

/** Player���� ����� �Լ��� */
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


/** Skill ZXCV -> 0123 �Ź� SKillâ�� �������� ���� ������ Update�Ǿ
    ������ 0123�� ��ų�� �ִٸ� ��ų ���                               */
void UCSkillComponent::ActivateSkill(const int32& slotNum)
{
    FALSE_RETURN(State->IsIdleMode());
    // #1. ��Ÿ�� üũ

    // #2. ��ų ����� ���Ⱑ �ִ��� Ȯ���ؼ� DataAsset ��������
    if (slotNum >= 0 && slotNum < SkillSlotDatas.Num())
    {
        // #3. TArray�� TMap�� 0 , 1 , 2 ,3 ����Ǿ��־ �� ��ȣ�� �°� �������� -> Key�� Value �Ѵ� �ʿ�
        const TMap<EWeaponStateType, int32>& SkillSlotData = SkillSlotDatas[slotNum];
        for (const auto& Pair : SkillSlotData)
        {
            EWeaponStateType WeaponType = Pair.Key;
            WeaponSkillIndex            = Pair.Value;

            if (WeaponSkillIndex == -1)
                return;

            // #4. ã�� Ű ���� �̿��Ͽ� SkillWeaponDatas���� DataAsset*�� ������
            ACWeapon** FoundSkillWepaon = SkillWeapons.Find(WeaponType);

            if (*FoundSkillWepaon != nullptr)
            {
                SkillWeaponNow = (*FoundSkillWepaon);
                break;
            }
        }
    }

    // #5. ��밡���� SkillWeaponData�� �ִٸ� 
    if (SkillWeaponNow != nullptr)
    {
        if (SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->CheckCanActivateSkill())
        {
            SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->SetMainSkillWidget(ACPlayerController::GetPlayerMainUI()->FindIndexSkillMainSlot(slotNum));
            // Weapon�� ���� ���̸� ��� Cache(����) �س��� ��ų ����� �����ֱ� 
            if (WeaponState->IsEquipped())
            {
                CacheWeapon = WeaponState->GetCurrentWeapon();
                WeaponState->UnequipWeapon();

                // ��ų Weapon���� ��� ���� �� ��ų ���
                WeaponState->EquipWeapon(SkillWeaponNow);
                if (WeaponSkillIndex != -1)
                    SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->Activate();
            }
            //  Weapon�� ���� ������ ������ �ٷ� ��ų �ߵ� �� �ٽ� Unarmed ���·�
            else
            {
                // ��ų Weapon���� ��� ���� �� ��ų ���
                WeaponState->EquipWeapon(SkillWeaponNow);
                if (WeaponSkillIndex != -1)
                    SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->Activate();
            }

            // #6. ��ų Weapon ��� ���̵��� ����
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

/** ��ų ��� ������ CacheWeapon���� �ٽ� ����ֱ� */
void UCSkillComponent::End_DoSkill()
{
    //
    // Dissolve ������
    //

    // #1. ���� ��ų�� �����ϰ� Visibility�� ����
    if (WeaponSkillIndex != -1)
    {
        SkillWeaponNow->GetWeaponSkill(WeaponSkillIndex)->End_DoSkill();
        SkillWeaponNow->SetSkillWeaponVisibility(false);
        WeaponState->UnequipWeapon();
    }

    // #2. ��ų ����� ������ ����ִ� Weapon���� ��ü�ϰ� Weapon �ٽ� ���̵���
    if (CacheWeapon != nullptr)
    {
        WeaponState->EquipWeapon(CacheWeapon);
        CacheWeapon->SetSkillWeaponVisibility(true);
    }

    // #3. ���� �ʱ�ȭ 
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