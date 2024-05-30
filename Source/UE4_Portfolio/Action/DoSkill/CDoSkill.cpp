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

	// #1. 스킬이 활성화되어있고 bCanRotate가 true일때만
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


	// #1. OwnerWeapon에다 SkillIndex를 전달하여 SkillData의 데미지를
	//     적용할 수 있도록 설정 ( 근접공격도 같이 하므로 )
	State->SetSkillMode();
	OwnerWeapon->SetSkillIndex(index);
	OwnerWeapon->ExecuteSkillLaunchAmountBroadCast(Data.LaunchAmount); // LaunchAmount넘겨주기
	
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

/** NotifyState -> 검을 휘두르기 시작할때의 위치 저장 */
void ACDoSkill::Begin_FromSlash()
{
	fromSlashVec = OwnerWeapon->GetActorLocation();
}

/** NotifyState -> 검을 휘두른 후의 위치 저장 */
void ACDoSkill::End_FromSlash()
{
	if (SlashClass == nullptr)
		return;

	FVector forward = CSub::GetCustomForwardVector(OwnerCharacter);
	forward *= 100.f;

	// #1. 캐릭터의 정면에서 검기 발사 위치
	endSlashVec =  OwnerWeapon->GetActorLocation();

	// #2. 검을 휘두르기 전 , 휘두른 후의 벡터방향을 이용하여 각도 구하기
	float degree = 0.f;
	{
		// #3. UpVector와 검을 휘두른 벡터를 내적하여 각도 구하기
		FVector slashVec = (endSlashVec - fromSlashVec);
		slashVec.Normalize();

		FVector upVec = CSub::GetCustomUpVector(OwnerCharacter);
		float dotProduct = slashVec | upVec;
		float dotAngle = acosf(dotProduct);

		// #4. 라디안 -> 각도로 변환
		dotAngle = dotAngle * (180.f / PI);

		// #5. YZ축에서의 SlashVec을 Pitch축 -90도 회전하여  
		//     XY축으로 옮긴 후 캐릭터 forward와 cross하여 왼오 구분
		slashVec.RotateAngleAxis(-90.f, CSub::GetCustomUpVector(OwnerCharacter));
		FVector crossProduct = slashVec ^ forward;

		// #6. 왼쪽일때 각도 예외처리
		if (crossProduct.Z > 0)
		{
			dotAngle = 180.f + (180.f - dotAngle);
		}

		dotAngle += 90.f; // 이건 slash niagara
		degree = dotAngle;
	}

	// #7. 구한 각도로 Spawn시키기
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
	// #1. 중복제거 , 적이 많을때를 위해 Set에 저장
	TSet<AActor*> HittedActors = OwnerWeapon->GetOverlappedActors();

	if (Data.ShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(Data.ShakeClass);
	}

	// #2. 스킬 사용중 검에 닿은 액터들의 State 파악 후 Dodge 아니면 데미지
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

/** 공격중에 bCanRotate가 true일때만 자연스럽게 회전하며 공격  */
void ACDoSkill::OwnerForwardRInterp(const float& deltaTime, const float& interpSpeed)
{
	// #1. Yaw부분만 뽑아온 Rotator 저장
	FRotator ownerRotator = OwnerCharacter->GetActorRotation();
	FRotator yawRotator = UKismetMathLibrary::MakeRotator(0, 0, OwnerCharacter->GetControlRotation().Yaw);
	
	// #2. UKismet은 블루프린트와 연계되어 오버헤드발생가능 -> FMath로 부드럽게 회전
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

