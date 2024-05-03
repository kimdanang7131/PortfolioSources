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
	
	// #1. 각 무기마다 BP에서 추가한 모든 Collision에 델리게이트를 붙여줌
	GetComponents<UShapeComponent>(ShapeComponents);
	for (UShapeComponent* component : ShapeComponents)
	{
		component->OnComponentBeginOverlap.AddDynamic(this, &ACWeapon::OnComponentBeginOverlap);
		component->OnComponentEndOverlap.AddDynamic(this, &ACWeapon::OnComponentEndOverlap);
	}

	// #2. 나머지도 BP에서 추가한 것들 가져오기
	GetComponents<USkeletalMeshComponent>(MeshComponents);
	GetComponents<UParticleSystemComponent>(ParticleSystemComponents);


	// #3. 시작시 설정 -> 무기 콜리젼 꺼주고, Weapon에 스킬이 있다면 생성
	OffCollision();
	CreateWeaponSkills();
}


// BP에서 사용할 붙이는 함수
void ACWeapon::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}


/// 정리필요
void ACWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TRUE_RETURN(OwnerCharacter == OtherActor);
	TRUE_RETURN(OwnerCharacter->GetClass() == OtherActor->GetClass());
	// 이미 맞은 캐릭터라면 return
	TRUE_RETURN(OwnerCharacter->GetClass() == OtherActor->GetOwner()->GetClass());

	TRUE_RETURN(OverlappedActors.Contains(OtherActor));
	OverlappedActors.Add(OtherActor);

	float power        = 0.f;
	float hitStop      = 0.f;
	TSubclassOf<UCameraShakeBase> shake;
	UNiagaraSystem* hitEffect;

	// 피격 전용 스킬모드일때의 설정 따로
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

	// #5. 구르기만 아니면 데미지 주기
	if (CSub::CustomTakeDamage(power, OtherActor, OwnerCharacter->GetController(), this))
	{
		// CameraShake 
		if (shake)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(shake);
		}
		// HitStop
        // #2. HitStop이 체크되어 있다면 0.001초동안 HitStop -> GlobalTimeDilation
		if (FMath::IsNearlyZero(hitStop) == false)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-1f); // 0.1f;
			UKismetSystemLibrary::K2_SetTimer(this, "RestoreDilation", hitStop * 1e-1f, false);
		}
		// Effect관련 처리
	}


	// Effect
	// #1. 부딪힌 위치로부터 설정한 위치에 추가되어 Effect 발생
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


///  아래서부터 Notify 관련 함수들 /////////////////////////////////////

void ACWeapon::OnCollision()
{
	for (UShapeComponent* component : ShapeComponents)
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACWeapon::OffCollision()
{
	for (UShapeComponent* component : ShapeComponents)
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 공격하고 나서 OffCollision시 남아있을수도 있어서
	OverlappedActors.Empty();
}

void ACWeapon::OnTrail()
{
	// #1. 스킬 모드 or 일반 공격 나눠서 거기에 맞춰 Trail 나오게 하기
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

// Action 도중 interrupt 당하여 End_DoAction을 하지 못하고 꺼졌을 예외 상황
void ACWeapon::ResetAction()
{
	// #1. 콤보 인덱스 및 다른것들 모두 초기화
	Index = 0;
	bCanCombo       = false;
	bComboActivated = false;

	//State->SetIdleMode();
	//Status->SetMove();
	//Status->SetControl();

	// #2. OnCollision에서 State로 OffCollision() 되기도 전에 끝났을 예외상황
	OffCollision();
}

void ACWeapon::DoAction()
{
	FALSE_RETURN(Datas.Num() > 0);
	FALSE_RETURN(Status->CheckCanDoAction(Datas[Index].Power)); // Stamina 확인

	// #1. bEnable이  NotifyState에 의하여 켜졌을 때 
	//     이 함수가 다시 들어온다면 Combo로 전환
	if (bCanCombo == true)
	{
		bComboActivated = true;
		bCanCombo   = false;

		return;
	}

	// Idle 모드 일때만 콤보 첫 공격부터 시작
	FALSE_RETURN(State->IsIdleMode());

	const FActionData& data = Datas[0];
	if (data.AnimMontage == nullptr)
	{
		CLog::Print("No ActionData Error : DoAction()");
		return;
	}

	//# 2. 공격상태 전환 후 스테미나 소비
	ActionSettings();

	State->SetActionMode();
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	// #3. 애니메이션이 움직일 수 있는 상태인지 DataAsset에서 체크된걸 가져와서 적용
	data.bCanMove ? Status->SetMove() : Status->SetStop();
}



// Notify를 통해 함수 실행 -> 콤보 가능한지 확인 후 다음 몽타주로 
void ACWeapon::Begin_DoAction()
{
	// #1. 콤보가 연결되지 않았거나 스테미나가 충분하지 않을 때
	FALSE_RETURN(bComboActivated);
	FALSE_RETURN(Status->CheckCanDoAction(Datas[Index].Power));

	// Player만 적용 ( Controller Yaw 방향만 가져와서 회전 적용 ) -> 부드럽진 않다
	if (OwnerCharacter->IsA<ACPlayer>())
	{
		FRotator controlRotation = OwnerCharacter->GetControlRotation();
		FRotator newRotation = FRotator(OwnerCharacter->GetActorRotation().Pitch, controlRotation.Yaw, OwnerCharacter->GetActorRotation().Roll);
		
		OwnerCharacter->SetActorRotation(newRotation);
	}

	// #2. 좌클릭을 눌러 콤보가 연결되었을 때 현재 몽타주 멈추고 다음 콤보의 몽타주 재생
	
	OwnerCharacter->StopAnimMontage();

	bComboActivated = false;
	Index++;

	ActionSettings();

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
	Status->SetControl();

	// #3. 콤보 초기화
	Index = 0;
}

void ACWeapon::ActionSettings()
{
	Status->SubStamina(Datas[Index].StaminaUsage);
	OnLaunchCharacterDelegate.Broadcast(Datas[Index].LaunchAmount);
}

///  여기까지 Notify 관련 함수들 /////////////////////////////////////



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