#include "Action/DoSkill/CDoSkill.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

#include "Action/CWeapon.h"
#include "Action/DoSkill/KatanaSkill/CActor_Sub_Slash.h"

ACDoSkill::ACDoSkill()
{
	PrimaryActorTick.bCanEverTick = true;

	SlashClass = ConstructorHelpers::FClassFinder<ACActor_Sub_Slash>(TEXT("Blueprint'/Game/Blueprints/DataAsset/Actors/SkillSubActors/BP_CActor_Sub_Slash.BP_CActor_Sub_Slash_C'")).Class;
}

void ACDoSkill::BeginPlay()
{
	// OwnerCharacter, State, Status
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	Super::BeginPlay();

	
}

void ACDoSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActivated)
	{
		if (bCanRotate)
		{
			OwnerFowardRInterp(DeltaTime, rInterpSpeed);
		}
	}
}

void ACDoSkill::End_Hold()
{
	if (bIsActivated)
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("endHold"), Data.AnimMontage);
	}
}

void ACDoSkill::Activate()
{
	bIsActivated = true;
	State->SetSkillMode();
	OwnerWeapon->SetSkillIndex(index);

	OwnerCharacter->PlayAnimMontage(Data.AnimMontage, Data.PlayRatio , Data.StartSection);
	
	Data.bCanMove ? Status->SetMove() : Status->SetStop();
	//Data.bCanControl ? Status->SetControl() : Status->SetStopControl();
}

void ACDoSkill::Deactivate()
{
	bIsActivated = false;

	OwnerCharacter->StopAnimMontage(Data.AnimMontage);
	State->SetIdleMode();
	Status->SetMove();
	//Status->SetControl();
}

// NotifyState -> 내려치기 시작할때의 위치 저장
void ACDoSkill::Begin_FromSlash()
{
	FromSlashVec = OwnerWeapon->GetActorLocation();
}

// NotifyState -> 내려치고나서의 위치 저장 및 검기 발사
void ACDoSkill::End_FromSlash()
{
	// #1. 캐릭터의 정면에서 검기를 발사하기 위해
	EndSlashVec = OwnerWeapon->GetActorLocation();
	FVector forward = CSub::GetCustomForwardVector(OwnerCharacter);
	forward *= 100.f;

	// #2. 검을 내려치기 전 , 내려친 후의 벡터방향을 이용하여 각도 구하기
	float degree = 0.f;
	{
		// #3. UpVector와 검을 휘두른 방향을 내적하여 각도 구하기
		FVector SlashVec = (EndSlashVec - FromSlashVec).GetSafeNormal();
		FVector upVec = CSub::GetCustomUpVector(OwnerCharacter);

		float dotProduct = SlashVec | upVec;
		float dotAngle = acos(dotProduct);

		// #4. 라디안 - 각도 변환
		dotAngle = (180.f / PI) * dotAngle;


		// #5. 외적하여 왼오 구분 -> Z축( 0,0,1 ) 기준으로 외적하기 때문에 2차원 평면 ( 원래는 X ,Y -> Z ,Y 평면 기준 X에서 오른손의 법칙이 발동됨 )
		FVector crossProduct = SlashVec ^ upVec;
		if (crossProduct.X < 0)
			dotAngle *= -1;

		dotAngle += 90.f;
		degree = dotAngle;
	}

	// #6. 구한 각도로 Spawn
	float OwnerYaw = OwnerCharacter->GetActorRotation().Yaw;

	FVector  SpawnLocation  = OwnerCharacter->GetActorLocation() + forward;
	FRotator SpawnRotation  = FRotator(0, OwnerYaw, degree);
	FVector  SpawnScale     = FVector(1, 1, 1);

	FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation, SpawnScale);

	ACActor_Sub_Slash* SlashActor = CHelpers::SpawnActorOnRuntime<ACActor_Sub_Slash>(SlashClass, OwnerCharacter, SpawnTransform);
}


void ACDoSkill::OnAttackByTimer()
{
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ACDoSkill::GetWeaponOverlappedActors, attackTimerTick, true);
}

void ACDoSkill::OffAttackByTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(Handle);
}

void ACDoSkill::GetWeaponOverlappedActors()
{
	//CLog::Print("GetWeaponDoSkill");
	TSet<AActor*> HittedActors = OwnerWeapon->GetOverlappedActors();

	FDamageEvent e;
	for (AActor* hittedActor : HittedActors)
		hittedActor->TakeDamage(Data.Power, e, OwnerCharacter->GetController(), this);
}

void ACDoSkill::OwnerFowardRInterp(const float& DeltaTime, const float& InterpSpeed)
{
	float OwnerControllerYaw = OwnerCharacter->GetControlRotation().Yaw;

	FRotator OwnerRotator = OwnerCharacter->GetActorRotation();
	FRotator YawRotator = UKismetMathLibrary::MakeRotator(0, 0, OwnerControllerYaw);

	FRotator InterpRotator = UKismetMathLibrary::RInterpTo(OwnerRotator, YawRotator, DeltaTime, InterpSpeed);

	OwnerCharacter->SetActorRotation(InterpRotator);
}


void ACDoSkill::test()
{
	//if (!!Data.AnimMontage)
	//	CLog::Print(Data.AnimMontage->GetName(), -1, 50.f);

	//if (!!Data.Power)
	//	CLog::Print(Data.Power, -1, 50.f);
}

