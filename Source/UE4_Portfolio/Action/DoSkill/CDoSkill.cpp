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

	//SlashClass = ConstructorHelpers::MyFClassFinder<ACActor_Sub_Slash>(TEXT("Blueprint'/Game/Blueprints/DataAsset/Actors/SkillSubActors/BP_CActor_Sub_Slash.BP_CActor_Sub_Slash_C'")).Class;
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

	// #1. ��ų�� Ȱ��ȭ�Ǿ��ְ� bCanRotate�� true�϶���
	if (bIsActivated)
	{
		if (bCanRotate)
			OwnerForwardRInterp(DeltaTime, rInterpSpeed);
	}
}

void ACDoSkill::End_Hold()
{
	if (bIsActivated)
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("endHold"), Data.AnimMontage);
}

void ACDoSkill::Activate()
{
	bIsActivated = true;

	// #1. OwnerWeapon���� SkillIndex�� �����Ͽ� SkillData�� ��������
	//     ������ �� �ֵ��� ���� ( �������ݵ� ���� �ϹǷ� )
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

// NotifyState -> �ֵθ��� �����Ҷ��� ��ġ ����
void ACDoSkill::Begin_FromSlash()
{
	fromSlashVec = OwnerWeapon->GetActorLocation();
}

// NotifyState -> �ֵθ��� ���� ��ġ ���� �� �˱� �߻�
void ACDoSkill::End_FromSlash()
{
	if (SlashClass == nullptr)
		return;

	// #1. ĳ������ ���鿡�� �˱� �߻� ��ġ
	endSlashVec = OwnerWeapon->GetActorLocation();
	FVector forward = CSub::GetCustomForwardVector(OwnerCharacter);
	forward *= 100.f;

	// #2. ���� �ֵθ��� �� , �ֵθ� ���� ���͹����� �̿��Ͽ� ���� ���ϱ�
	float degree = 0.f;
	{
		// #3. UpVector�� ���� �ֵθ� ������ �����Ͽ� ���� ���ϱ�
		FVector slashVec = (endSlashVec - fromSlashVec).GetSafeNormal();
		FVector upVec = CSub::GetCustomUpVector(OwnerCharacter);

		float dotProduct = slashVec | upVec;
		float dotAngle = acos(dotProduct);

		// #4. ���� - ���� ��ȯ
		dotAngle = (180.f / PI) * dotAngle;

		// #5. YZ�� ���� ���� -> X�� �������� ��Ģ
		FVector crossProduct = slashVec ^ upVec;
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

	ACActor_Sub_Slash* SlashActor = CHelpers::MySpawnActor<ACActor_Sub_Slash>(SlashClass, OwnerCharacter, SpawnTransform);
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
	// #1. �ߺ����� , ���� �������� ���� Set�� ����
	TSet<AActor*> HittedActors = OwnerWeapon->GetOverlappedActors();

	// #2. ��ų ����� �˿� ���� ���͵��� State �ľ� �� Dodge �ƴϸ� ������
	FDamageEvent e;
	for (AActor* hittedActor : HittedActors)
	{
		UCStateComponent* TargetState = Cast<UCStateComponent>(hittedActor->GetComponentByClass(UCStateComponent::StaticClass()));
		
		if (!TargetState->IsDodgeMode())
			hittedActor->TakeDamage(Data.Power, e, OwnerCharacter->GetController(), this);
	}
		
}

// #1. �����߿� bCanRotate�� true�϶��� �ڿ������� ȸ���ϸ� ����
void ACDoSkill::OwnerForwardRInterp(const float& deltaTime, const float& interpSpeed)
{
	// #2. Yaw�κи� �̾ƿ� Rotator ����
	FRotator ownerRotator = OwnerCharacter->GetActorRotation();
	FRotator yawRotator = UKismetMathLibrary::MakeRotator(0, 0, OwnerCharacter->GetControlRotation().Yaw);
	
	// #3. UKismet�� �������Ʈ�� ����Ǿ� �������߻����� -> FMath�� �ε巴�� ȸ��
	FRotator interpedRotator = FMath::RInterpTo(ownerRotator, yawRotator , deltaTime, interpSpeed);
	OwnerCharacter->SetActorRotation(interpedRotator);
}




void ACDoSkill::test()
{
	//if (!!Data.AnimMontage)
	//	CLog::Print(Data.AnimMontage->GetName(), -1, 50.f);

	//if (!!Data.Power)
	//	CLog::Print(Data.Power, -1, 50.f);
}

