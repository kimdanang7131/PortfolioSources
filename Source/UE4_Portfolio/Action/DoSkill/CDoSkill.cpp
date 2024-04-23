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

// NotifyState -> ����ġ�� �����Ҷ��� ��ġ ����
void ACDoSkill::Begin_FromSlash()
{
	FromSlashVec = OwnerWeapon->GetActorLocation();
}

// NotifyState -> ����ġ������ ��ġ ���� �� �˱� �߻�
void ACDoSkill::End_FromSlash()
{
	// #1. ĳ������ ���鿡�� �˱⸦ �߻��ϱ� ����
	EndSlashVec = OwnerWeapon->GetActorLocation();
	FVector forward = CSub::GetCustomForwardVector(OwnerCharacter);
	forward *= 100.f;

	// #2. ���� ����ġ�� �� , ����ģ ���� ���͹����� �̿��Ͽ� ���� ���ϱ�
	float degree = 0.f;
	{
		// #3. UpVector�� ���� �ֵθ� ������ �����Ͽ� ���� ���ϱ�
		FVector SlashVec = (EndSlashVec - FromSlashVec).GetSafeNormal();
		FVector upVec = CSub::GetCustomUpVector(OwnerCharacter);

		float dotProduct = SlashVec | upVec;
		float dotAngle = acos(dotProduct);

		// #4. ���� - ���� ��ȯ
		dotAngle = (180.f / PI) * dotAngle;


		// #5. �����Ͽ� �޿� ���� -> Z��( 0,0,1 ) �������� �����ϱ� ������ 2���� ��� ( ������ X ,Y -> Z ,Y ��� ���� X���� �������� ��Ģ�� �ߵ��� )
		FVector crossProduct = SlashVec ^ upVec;
		if (crossProduct.X < 0)
			dotAngle *= -1;

		dotAngle += 90.f;
		degree = dotAngle;
	}

	// #6. ���� ������ Spawn
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

