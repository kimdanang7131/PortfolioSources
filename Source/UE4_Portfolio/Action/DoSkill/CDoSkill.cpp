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

	// #1. 스킬이 활성화되어있고 bCanRotate가 true일때만
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

	// #1. OwnerWeapon에다 SkillIndex를 전달하여 SkillData의 데미지를
	//     적용할 수 있도록 설정 ( 근접공격도 같이 하므로 )
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

// NotifyState -> 휘두르기 시작할때의 위치 저장
void ACDoSkill::Begin_FromSlash()
{
	fromSlashVec = OwnerWeapon->GetActorLocation();
}

// NotifyState -> 휘두르고 나서 위치 저장 및 검기 발사
void ACDoSkill::End_FromSlash()
{
	if (SlashClass == nullptr)
		return;

	// #1. 캐릭터의 정면에서 검기 발사 위치
	endSlashVec = OwnerWeapon->GetActorLocation();
	FVector forward = CSub::GetCustomForwardVector(OwnerCharacter);
	forward *= 100.f;

	// #2. 검을 휘두르기 전 , 휘두른 후의 벡터방향을 이용하여 각도 구하기
	float degree = 0.f;
	{
		// #3. UpVector와 검을 휘두른 방향을 내적하여 각도 구하기
		FVector slashVec = (endSlashVec - fromSlashVec).GetSafeNormal();
		FVector upVec = CSub::GetCustomUpVector(OwnerCharacter);

		float dotProduct = slashVec | upVec;
		float dotAngle = acos(dotProduct);

		// #4. 라디안 - 각도 변환
		dotAngle = (180.f / PI) * dotAngle;

		// #5. YZ축 기준 외적 -> X축 오른손의 법칙
		FVector crossProduct = slashVec ^ upVec;
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
	// #1. 중복제거 , 적이 많을때를 위해 Set에 저장
	TSet<AActor*> HittedActors = OwnerWeapon->GetOverlappedActors();

	// #2. 스킬 사용중 검에 닿은 액터들의 State 파악 후 Dodge 아니면 데미지
	FDamageEvent e;
	for (AActor* hittedActor : HittedActors)
	{
		UCStateComponent* TargetState = Cast<UCStateComponent>(hittedActor->GetComponentByClass(UCStateComponent::StaticClass()));
		
		if (!TargetState->IsDodgeMode())
			hittedActor->TakeDamage(Data.Power, e, OwnerCharacter->GetController(), this);
	}
		
}

// #1. 공격중에 bCanRotate가 true일때만 자연스럽게 회전하며 공격
void ACDoSkill::OwnerForwardRInterp(const float& deltaTime, const float& interpSpeed)
{
	// #2. Yaw부분만 뽑아온 Rotator 저장
	FRotator ownerRotator = OwnerCharacter->GetActorRotation();
	FRotator yawRotator = UKismetMathLibrary::MakeRotator(0, 0, OwnerCharacter->GetControlRotation().Yaw);
	
	// #3. UKismet은 블루프린트와 연계되어 오버헤드발생가능 -> FMath로 부드럽게 회전
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

