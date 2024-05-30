#include "Action/DoSkill/CDoSkill.h"
#include "Global.h"
//////////////////////
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
//////////////////////
#include "Action/DoSkill/KatanaSkill/CActor_Sub_Slash.h"
#include "Action/CWeapon.h"
#include "Widgets/CUserWidget_MainSkillSlot.h"
//////////////////////
#include "GameFramework/Character.h"

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

	if (bOnCoolTime)
	{
		spendTime -= DeltaTime;

		if ( spendTime < 0.f )
		{
			if (MainSkillSlot)
				MainSkillSlot->SetProgressTranscluent(false);
			spendTime = 0.f;
			bOnCoolTime = false;
			MainSkillSlot = nullptr;
		}

		if(MainSkillSlot)
			MainSkillSlot->SetCoolPercent(spendTime / coolTime);
	}
}

void ACDoSkill::End_Hold()
{
	if (bIsActivated)
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("endHold"), Data.AnimMontage);
}

void ACDoSkill::Activate()
{
	bOnCoolTime = true;
	bIsActivated = true;
	spendTime = coolTime;

	if (MainSkillSlot)
		MainSkillSlot->SetProgressTranscluent(true);


	// #1. OwnerWeapon���� SkillIndex�� �����Ͽ� SkillData�� ��������
	//     ������ �� �ֵ��� ���� ( �������ݵ� ���� �ϹǷ� )
	State->SetSkillMode();
	OwnerWeapon->SetSkillIndex(index);
	OwnerWeapon->ExecuteSkillLaunchAmountBroadCast(Data.LaunchAmount); // LaunchAmount�Ѱ��ֱ�
	
	if (Data.AnimMontage == nullptr)
	{
		CLog::Print("Activate() Error!");
		return;
	}

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
	Status->SetControl();
}

/** NotifyState -> ���� �ֵθ��� �����Ҷ��� ��ġ ���� */
void ACDoSkill::Begin_FromSlash()
{
	fromSlashVec = OwnerWeapon->GetActorLocation();
}

/** NotifyState -> ���� �ֵθ� ���� ��ġ ���� */
void ACDoSkill::End_FromSlash()
{
	if (SlashClass == nullptr)
		return;

	FVector forward = CSub::GetCustomForwardVector(OwnerCharacter);
	forward *= 100.f;

	// #1. ĳ������ ���鿡�� �˱� �߻� ��ġ
	endSlashVec =  OwnerWeapon->GetActorLocation();

	// #2. ���� �ֵθ��� �� , �ֵθ� ���� ���͹����� �̿��Ͽ� ���� ���ϱ�
	float degree = 0.f;
	{
		// #3. UpVector�� ���� �ֵθ� ���͸� �����Ͽ� ���� ���ϱ�
		FVector slashVec = (endSlashVec - fromSlashVec);
		slashVec.Normalize();

		FVector upVec = CSub::GetCustomUpVector(OwnerCharacter);
		float dotProduct = slashVec | upVec;
		float dotAngle = acosf(dotProduct);

		// #4. ���� -> ������ ��ȯ
		dotAngle = dotAngle * (180.f / PI);

		// #5. YZ�࿡���� SlashVec�� Pitch�� -90�� ȸ���Ͽ�  
		//     XY������ �ű� �� ĳ���� forward�� cross�Ͽ� �޿� ����
		slashVec.RotateAngleAxis(-90.f, CSub::GetCustomUpVector(OwnerCharacter));
		FVector crossProduct = slashVec ^ forward;

		// #6. �����϶� ���� ����ó��
		if (crossProduct.Z > 0)
		{
			dotAngle = 180.f + (180.f - dotAngle);
		}

		dotAngle += 90.f; // �̰� slash niagara
		degree = dotAngle;
	}

	// #7. ���� ������ Spawn��Ű��
	FVector  SpawnLocation  = OwnerCharacter->GetActorLocation() + forward;
	FRotator SpawnRotation  = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, degree);
	FVector  SpawnScale     = FVector(1, 1, 1);

	FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation, SpawnScale);

	ACActor_Sub_Slash* SlashActor = CHelpers::MySpawnActor<ACActor_Sub_Slash>(SlashClass, OwnerCharacter, SpawnTransform);
}

//DrawDebugDirectionalArrow(GetWorld(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + slashVec * 100, 25.f, FColor::Red, true, -1.f, 0U, 3.f);
//DrawDebugDirectionalArrow(GetWorld(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + upVec * 100, 25.f, FColor::Yellow, true, -1.f, 0U, 3.f);
//DrawDebugDirectionalArrow(GetWorld(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + slashVec * 100, 25.f, FColor::Green, true, -1.f, 0U, 3.f);
//DrawDebugDirectionalArrow(GetWorld(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + forward * 10, 25.f, FColor::Green, true, -1.f, 0U, 3.f);



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

	if (Data.ShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(Data.ShakeClass);
	}

	// #2. ��ų ����� �˿� ���� ���͵��� State �ľ� �� Dodge �ƴϸ� ������
	FDamageEvent e;

	for (AActor* hittedActor : HittedActors)
	{
		UCStateComponent* TargetState = Cast<UCStateComponent>(hittedActor->GetComponentByClass(UCStateComponent::StaticClass()));
		
		if (TargetState)
		{
			if (!TargetState->IsDodgeMode())
				hittedActor->TakeDamage(Data.Power, e, OwnerCharacter->GetController(), this);
		}
	}
}

/** �����߿� bCanRotate�� true�϶��� �ڿ������� ȸ���ϸ� ����  */
void ACDoSkill::OwnerForwardRInterp(const float& deltaTime, const float& interpSpeed)
{
	// #1. Yaw�κи� �̾ƿ� Rotator ����
	FRotator ownerRotator = OwnerCharacter->GetActorRotation();
	FRotator yawRotator = UKismetMathLibrary::MakeRotator(0, 0, OwnerCharacter->GetControlRotation().Yaw);
	
	// #2. UKismet�� �������Ʈ�� ����Ǿ� �������߻����� -> FMath�� �ε巴�� ȸ��
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

bool ACDoSkill::CheckCanActivateSkill()
{
	return spendTime <= 0.f;
}

