#include "Action/CWeapon.h"
#include "Global.h"

#include "Characters/CPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
//#include "Components/CWeaponStateComponent.h" 

#include "Action/DoSkill/CDoSkill.h"

/////////////////////////////////////////////
#include "GameFramework/Character.h"

#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraShakeBase.h"
#include "MatineeCameraShake.h"
#include "CMatineeCameraShake.h"

#include "Particles/ParticleSystemComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SceneComponent.h"



ACWeapon::ACWeapon()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
}

void ACWeapon::BeginPlay()
{
	// OwnerCharacter, State, Status , WeaponState
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State  = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	WeaponState = CHelpers::GetComponent<UCWeaponStateComponent>(OwnerCharacter);

	Super::BeginPlay();

	//GetComponents<UParticleSystemComponent>()
	
	// #1. �� ���⸶�� BP���� �߰��� ��� Collision�� ��������Ʈ�� �ٿ���
	GetComponents<UShapeComponent>(ShapeComponents);
	for (UShapeComponent* component : ShapeComponents)
	{
		component->OnComponentBeginOverlap.AddDynamic(this, &ACWeapon::OnComponentBeginOverlap);
		component->OnComponentEndOverlap.AddDynamic(this, &ACWeapon::OnComponentEndOverlap);
	}

	// #2. �������� BP���� �߰��� �͵� ��������
	GetComponents<USkeletalMeshComponent>(MeshComponents);
	GetComponents<UParticleSystemComponent>(ParticleSystemComponents);


	// #3. ���۽� ���� -> ���� �ݸ��� ���ְ�, Weapon�� ��ų�� �ִٸ� ����
	OffCollision();
	CreateWeaponSkills();
}


// BP���� ����� ���̴� �Լ�
void ACWeapon::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}


/// �����ʿ�
void ACWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TRUE_RETURN(OwnerCharacter == OtherActor);
	TRUE_RETURN(OwnerCharacter->GetClass() == OtherActor->GetClass());
	// �̹� ���� ĳ���Ͷ�� return
	TRUE_RETURN(OwnerCharacter->GetClass() == OtherActor->GetOwner()->GetClass());

	TRUE_RETURN(OverlappedActors.Contains(OtherActor));
	OverlappedActors.Add(OtherActor);

	float power        = 0.f;
	float hitStop      = 0.f;
	TSubclassOf<UCameraShakeBase> shake;
	UNiagaraSystem* hitEffect;

	// �ǰ� ���� ��ų����϶��� ���� ����
	if (State->IsSkillMode())
	{
		power        = SkillDatas[SkillIndex].Power;
		hitStop      = SkillDatas[SkillIndex].HitStop;
		shake        = SkillDatas[SkillIndex].ShakeClass;
		hitEffect    = SkillDatas[SkillIndex].Effect;
	}
	else
	{
		power        = Datas[Index].Power;
		hitStop      = Datas[Index].HitStop;
		shake        = Datas[Index].ShakeClass;
		hitEffect    = Datas[Index].Effect;
	}

	// #5. �����⸸ �ƴϸ� ������ �ֱ�
	if (CSub::CustomTakeDamage(power, OtherActor, OwnerCharacter->GetController(), this))
	{
		// CameraShake 
		if (shake)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(shake);
		}
		// HitStop
        // #2. HitStop�� üũ�Ǿ� �ִٸ� 0.001�ʵ��� HitStop -> GlobalTimeDilation
		if (FMath::IsNearlyZero(hitStop) == false)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-1f); // 0.1f;
			UKismetSystemLibrary::K2_SetTimer(this, "RestoreDilation", hitStop * 1e-1f, false);
		}
		// Effect���� ó��
	}


	// Effect
	// #1. �ε��� ��ġ�κ��� ������ ��ġ�� �߰��Ǿ� Effect �߻�
	//UParticleSystem* hitEffect = Datas[Index].Effect;
	//if (!!hitEffect)
	//{
	//	FTransform transform = Datas[Index].EffectTransform;
	//	transform.AddToTranslation(OtherActor->GetActorLocation());
	//
	//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	//}
}

void ACWeapon::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OverlappedActors.Contains(OtherActor))
	{
		OverlappedActors.Remove(OtherActor);
	}
}


USkeletalMeshComponent* ACWeapon::GetOwnerMesh()
{
	if (OwnerCharacter == nullptr)
	{
		CLog::Print("Weapon->GetOwnerMesh() Error!");
		if (OwnerCharacter->GetMesh())
			return nullptr;

		return nullptr;
	}

	return OwnerCharacter->GetMesh();
}


///  �Ʒ������� Notify ���� �Լ��� /////////////////////////////////////

void ACWeapon::OnCollision()
{
	for (UShapeComponent* component : ShapeComponents)
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACWeapon::OffCollision()
{
	for (UShapeComponent* component : ShapeComponents)
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// �����ϰ� ���� OffCollision�� ������������ �־
	OverlappedActors.Empty();
}

void ACWeapon::OnTrail()
{
	// #1. ��ų ��� or �Ϲ� ���� ������ �ű⿡ ���� Trail ������ �ϱ�
	if (!!Datas[Index].Trail)
	{
		if (State->IsSkillMode())
		{
			for (UParticleSystemComponent* particleSystem : ParticleSystemComponents)
			{
				particleSystem->SetTemplate(SkillDatas[SkillIndex].Trail);
				particleSystem->BeginTrails("center", "end", ETrailWidthMode::ETrailWidthMode_FromFirst, 1.0f);
			}
		}
		else
		{
			for (UParticleSystemComponent* particleSystem : ParticleSystemComponents)
			{
				particleSystem->SetTemplate(Datas[Index].Trail);
				particleSystem->BeginTrails("center", "end", ETrailWidthMode::ETrailWidthMode_FromFirst, 1.0f);
			}
		}
	}
}

void ACWeapon::OffTrail()
{
	if (!!Datas[Index].Trail)
	{
		for (UParticleSystemComponent* particleSystem : ParticleSystemComponents)
			particleSystem->EndTrails();
	}
}

// Action ���� interrupt ���Ͽ� End_DoAction�� ���� ���ϰ� ������ ���� ��Ȳ
void ACWeapon::ResetAction()
{
	// #1. �޺� �ε��� �� �ٸ��͵� ��� �ʱ�ȭ
	Index = 0;
	bCanCombo       = false;
	bComboActivated = false;

	//State->SetIdleMode();
	//Status->SetMove();
	//Status->SetControl();

	// #2. OnCollision���� State�� OffCollision() �Ǳ⵵ ���� ������ ���ܻ�Ȳ
	OffCollision();
}

void ACWeapon::DoAction()
{
	FALSE_RETURN(Datas.Num() > 0);
	FALSE_RETURN(Status->CheckCanDoAction(Datas[Index].Power)); // Stamina Ȯ��

	// #1. bEnable��  NotifyState�� ���Ͽ� ������ �� 
	//     �� �Լ��� �ٽ� ���´ٸ� Combo�� ��ȯ
	if (bCanCombo == true)
	{
		bComboActivated = true;
		bCanCombo   = false;

		return;
	}

	// Idle ��� �϶��� �޺� ù ���ݺ��� ����
	FALSE_RETURN(State->IsIdleMode());

	const FActionData& data = Datas[0];
	if (data.AnimMontage == nullptr)
	{
		CLog::Print("No ActionData Error : DoAction()");
		return;
	}

	//# 2. ���ݻ��� ��ȯ �� ���׹̳� �Һ�
	ActionSettings();

	State->SetActionMode();
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	// #3. �ִϸ��̼��� ������ �� �ִ� �������� DataAsset���� üũ�Ȱ� �����ͼ� ����
	data.bCanMove ? Status->SetMove() : Status->SetStop();
}



// Notify�� ���� �Լ� ���� -> �޺� �������� Ȯ�� �� ���� ��Ÿ�ַ� 
void ACWeapon::Begin_DoAction()
{
	// #1. �޺��� ������� �ʾҰų� ���׹̳��� ������� ���� ��
	FALSE_RETURN(bComboActivated);
	FALSE_RETURN(Status->CheckCanDoAction(Datas[Index].Power));

	// Player�� ���� ( Controller Yaw ���⸸ �����ͼ� ȸ�� ���� ) -> �ε巴�� �ʴ�
	if (OwnerCharacter->IsA<ACPlayer>())
	{
		FRotator controlRotation = OwnerCharacter->GetControlRotation();
		FRotator newRotation = FRotator(OwnerCharacter->GetActorRotation().Pitch, controlRotation.Yaw, OwnerCharacter->GetActorRotation().Roll);
		
		OwnerCharacter->SetActorRotation(newRotation);
	}

	// #2. ��Ŭ���� ���� �޺��� ����Ǿ��� �� ���� ��Ÿ�� ���߰� ���� �޺��� ��Ÿ�� ���
	
	OwnerCharacter->StopAnimMontage();

	bComboActivated = false;
	Index++;

	ActionSettings();

	// #3. ������ ���¿� ������ �޺� ��Ÿ�� �����ͼ� ����
	const FActionData& data = Datas[Index];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	data.bCanMove ? Status->SetMove() : Status->SetStop();
}

// �޺� �������� �ʾ��� ��
void ACWeapon::End_DoAction()
{
	// #1. ���� �������� ��Ÿ�� ����
	const FActionData& data = Datas[Index];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	// #2. State & Status �ʱ�ȭ
	State->SetIdleMode();
	Status->SetMove();
	Status->SetControl();

	// #3. �޺� �ʱ�ȭ
	Index = 0;
}

void ACWeapon::ActionSettings()
{
	Status->SubStamina(Datas[Index].StaminaUsage);
	OnLaunchCharacterDelegate.Broadcast(Datas[Index].LaunchAmount);
}

///  ������� Notify ���� �Լ��� /////////////////////////////////////



void ACWeapon::RestoreDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}


////

void ACWeapon::CreateWeaponSkills()
{
	for (int i = 0; i < SkillDatas.Num(); i++)
	{
		FActorSpawnParameters spawnParams;
		
		spawnParams.Owner      = OwnerCharacter;
		spawnParams.Instigator = OwnerCharacter;
		ACDoSkill* weaponSkill = GetWorld()->SpawnActor<ACDoSkill>(SkillDatas[i].SkillClass, spawnParams);
		weaponSkill->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
		weaponSkill->SetData(SkillDatas[i]);
		weaponSkill->SetOwnerWeapon(this);
		
		WeaponSkills.Add(weaponSkill);
	}
}

ACDoSkill* ACWeapon::GetWeaponSkill(const int32& num)
{
	if (WeaponSkills.IsValidIndex(num) == false)
	{
		CLog::Print("Invalid GetWeaponSkill()");
		return nullptr;
	}

	return WeaponSkills[num];
}

void ACWeapon::SetSkillWeaponVisibility(bool bIsVisible)
{
	for (USkeletalMeshComponent* Mesh : MeshComponents)
		Mesh->SetVisibility(bIsVisible);
}