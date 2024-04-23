#include "Action/CWeapon.h"
#include "Global.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CWeaponStateComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/ShapeComponent.h"

#include "GameFramework/Character.h"

#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Particles/ParticleSystemComponent.h"

#include "Action/DoSkill/CDoSkill.h"

#include "Components/SceneComponent.h"

ACWeapon::ACWeapon()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
}

void ACWeapon::BeginPlay()
{
	// OwnerCharacter, State, Status
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	State  = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	WeaponState = CHelpers::GetComponent<UCWeaponStateComponent>(OwnerCharacter);

	Super::BeginPlay();

	//GetComponents<UParticleSystemComponent>()

	GetComponents<UShapeComponent>(ShapeComponents);
	for (UShapeComponent* component : ShapeComponents)
	{
		component->OnComponentBeginOverlap.AddDynamic(this, &ACWeapon::OnComponentBeginOverlap);
		component->OnComponentEndOverlap.AddDynamic(this, &ACWeapon::OnComponentEndOverlap);
	}

	GetComponents<USkeletalMeshComponent>(MeshComponents);
	GetComponents<UParticleSystemComponent>(ParticleSystemComponents);

	OffCollision();

	CreateWeaponSkills();
}

void ACWeapon::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}



void ACWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlaapedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TRUE_RETURN(OwnerCharacter == OtherActor);
	TRUE_RETURN(OwnerCharacter->GetClass() == OtherActor->GetClass());

	// 이미 맞은 캐릭터라면 return
	TRUE_RETURN(OverlappedActors.Contains(OtherActor));

	OverlappedActors.Add(OtherActor);

	// Effect
	UParticleSystem* hitEffect = Datas[Index].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[Index].EffectTransform;
		transform.AddToTranslation(OtherActor->GetActorLocation());

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	// HitStop
	float hitStop = Datas[Index].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-3f); // 0.001f;
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreDilation", hitStop * 1e-3f, false);
	}

	// CameraShake
	TSubclassOf<UCameraShakeBase> shake = Datas[Index].ShakeClass;
	if (!!shake)
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(shake);


	// 데미지 설정
	float Power = Datas[Index].Power;

	if (State->IsSkillMode())
		Power = SkillDatas[SkillIndex].Power;

	FDamageEvent e;
	OtherActor->TakeDamage(Power, e, OwnerCharacter->GetController(), this);
}

void ACWeapon::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//TRUE_RETURN(OwnerCharacter == OtherActor);
	//TRUE_RETURN(OwnerCharacter->GetClass() == OtherActor->GetClass());

	// -> SkillComp에서 SelectedSkillWeapon설정되었을때
	// OverlappedActors와  나간 OtherActor비교해서 빼주기
}

void ACWeapon::OnCollision()
{
	for (UShapeComponent* component : ShapeComponents)
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACWeapon::OffCollision()
{
	for (UShapeComponent* component : ShapeComponents)
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OverlappedActors.Empty();
}

void ACWeapon::OnTrail()
{
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
		{
			particleSystem->EndTrails();
		}
	}
}

void ACWeapon::DoAction()
{
	FALSE_RETURN(Datas.Num() > 0);
	FALSE_RETURN(Status->CheckCanDoAction(Datas[Index].Power));

	if (bEnable == true)
	{
		bCanCombo = true;
		bEnable = false;

		return;
	}

	// Idle 모드 일때만 콤보 첫 공격부터 시작
	FALSE_RETURN(State->IsIdleMode());
	Status->SubStamina(Datas[Index].Power);

	State->SetActionMode();

	const FActionData& data = Datas[0];
	if (!!data.AnimMontage)
	{
		//CLog::Print(data.AnimMontage->GetName());
	}

	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	data.bCanMove ? Status->SetMove() : Status->SetStop();
}


// Notify를 통해 함수 실행 -> 콤보 가능한지 확인 후 다음 몽타주로 
void ACWeapon::Begin_DoAction()
{
	// #1. 콤보가 연결되지 않았거나 스테미나가 충분하지 않을 때
	FALSE_RETURN(bCanCombo);
	FALSE_RETURN(Status->CheckCanDoAction(Datas[Index].Power));



	// #2. 좌클릭을 눌러 콤보가 연결되었을 때 현재 몽타주 멈추고 다음 콤보의 몽타주 재생
	Status->SubStamina(Datas[Index].Power);
	OwnerCharacter->StopAnimMontage();

	bCanCombo = false;
	Index++;
	
	// #3. 데이터 에셋에 설정된 콤보 몽타주 가져와서 실행
	const FActionData& data = Datas[Index];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	data.bCanMove ? Status->SetMove() : Status->SetStop();
}

// 콤보 이행하지 않았을 때
void ACWeapon::End_DoAction()
{
	// #1. 현재 실행중인 몽타주 정지
	const FActionData& data = Datas[Index];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	// #2. State & Status 초기화
	State->SetIdleMode();
	Status->SetMove();

	// #3. 콤보 초기화
	Index = 0;
}

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


void ACWeapon::SetSkillWeaponVisibility(bool bIsVisible)
{
	for (USkeletalMeshComponent* Mesh : MeshComponents)
	{
		Mesh->SetVisibility(bIsVisible);
	}
}