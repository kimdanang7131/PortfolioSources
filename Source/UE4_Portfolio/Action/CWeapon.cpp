#include "Action/CWeapon.h"
#include "Global.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CWeaponStateComponent.h"

#include "Action/DoSkill/CDoSkill.h"

#include "GameFramework/Character.h"

#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Particles/ParticleSystemComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SceneComponent.h"

#include "Characters/CPlayer.h"

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


void ACWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TRUE_RETURN(OwnerCharacter == OtherActor);
	TRUE_RETURN(OwnerCharacter->GetClass() == OtherActor->GetClass());
	// �̹� ���� ĳ���Ͷ�� return
	TRUE_RETURN(OwnerCharacter->GetClass() == OtherActor->GetOwner()->GetClass());

	TRUE_RETURN(OverlappedActors.Contains(OtherActor));
	OverlappedActors.Add(OtherActor);

	// Effect
	// #1. �ε��� ��ġ�κ��� ������ ��ġ�� �߰��Ǿ� Effect �߻�
	UParticleSystem* hitEffect = Datas[Index].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[Index].EffectTransform;
		transform.AddToTranslation(OtherActor->GetActorLocation());

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	// HitStop
	// #2. HitStop�� üũ�Ǿ� �ִٸ� 0.001�ʵ��� HitStop -> GlobalTimeDilation
	float hitStop = Datas[Index].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-3f); // 0.001f;
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreDilation", hitStop * 1e-3f, false);
	}


	// CameraShake 
	// #3. �����Ҷ��� �ߵ� / ��ų�� ����
	if (!State->IsSkillMode())
	{
		TSubclassOf<UCameraShakeBase> shake = Datas[Index].ShakeClass;
		if (!!shake)
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(shake);
	}

	// ������ ����
	// #4. �Ϲݰ��� / ��ų���� ������ �������� �ֵ���
	float Power = Datas[Index].Power;
	if (State->IsSkillMode())
		Power = SkillDatas[SkillIndex].Power;


	// #5. �����⸸ �ƴϸ� ������ �ֱ�
	UCStateComponent* TargetState = Cast<UCStateComponent>(OtherActor->GetComponentByClass(UCStateComponent::StaticClass()));

	FDamageEvent e;
	if (!TargetState->IsDodgeMode())
		OtherActor->TakeDamage(Power, e, OwnerCharacter->GetController(), this);
}

void ACWeapon::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
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

void ACWeapon::DoAction()
{
	FALSE_RETURN(Datas.Num() > 0);
	FALSE_RETURN(Status->CheckCanDoAction(Datas[Index].Power)); // Stamina Ȯ��

	// #1. bEnable��  NotifyState�� ���Ͽ� ������ �� 
	//     �� �Լ��� �ٽ� ���´ٸ� Combo�� ��ȯ
	if (bEnable == true)
	{
		bCanCombo = true;
		bEnable   = false;

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
	State->SetActionMode();
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
	Status->SubStamina(Datas[Index].Power);

	// #3. �ִϸ��̼��� ������ �� �ִ� �������� DataAsset���� üũ�Ȱ� �����ͼ� ����
	data.bCanMove ? Status->SetMove() : Status->SetStop();
}



// Notify�� ���� �Լ� ���� -> �޺� �������� Ȯ�� �� ���� ��Ÿ�ַ� 
void ACWeapon::Begin_DoAction()
{
	// #1. �޺��� ������� �ʾҰų� ���׹̳��� ������� ���� ��
	FALSE_RETURN(bCanCombo);
	FALSE_RETURN(Status->CheckCanDoAction(Datas[Index].Power));

	if (OwnerCharacter->IsA<ACPlayer>())
	{
		FRotator controlRotation = OwnerCharacter->GetControlRotation();
		FRotator newRotation = FRotator(OwnerCharacter->GetActorRotation().Pitch, controlRotation.Yaw, OwnerCharacter->GetActorRotation().Roll);
		
		OwnerCharacter->SetActorRotation(newRotation);
	}

	// #2. ��Ŭ���� ���� �޺��� ����Ǿ��� �� ���� ��Ÿ�� ���߰� ���� �޺��� ��Ÿ�� ���
	Status->SubStamina(Datas[Index].Power);
	OwnerCharacter->StopAnimMontage();

	bCanCombo = false;
	Index++;
	
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

	// #3. �޺� �ʱ�ȭ
	Index = 0;
}

///  ������� Notify ���� �Լ��� /////////////////////////////////////



void ACWeapon::RestoreDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

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