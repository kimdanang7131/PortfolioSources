#include "Action/DoSkill/OneHandSkill/CDoSkill_OneHand_02.h"
#include "Global.h"
////////////////////////
#include "Components/CPostProcessComponent.h"
////////////////////////
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
////////////////////////

ACDoSkill_OneHand_02::ACDoSkill_OneHand_02()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	Box   = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(Scene);

	Decal = CreateDefaultSubobject<UDecalComponent>("Decal");
	Decal->SetupAttachment(Scene);

	index = 1;
	coolTime = 11.f;
}

/** 스킬 시전 중에 움직일수 있도록 설정 */
void ACDoSkill_OneHand_02::BeginPlay()
{
	Super::BeginPlay();
	bCanRotate = true;

	OnActorBeginOverlap.AddDynamic(this, &ACDoSkill_OneHand_02::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACDoSkill_OneHand_02::ActorEndOverlap);
}

void ACDoSkill_OneHand_02::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


/** 홀딩 중 회전하는 동안 Skill Range가 함께 회전하며 Box에 닿은 순간 
    PostProcess로 하이라이트 시키면서 범위 안에 있는지 알려줌 */
void ACDoSkill_OneHand_02::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(OwnerCharacter, OtherActor));

	// #1. 추가 할 OtherActor의 포스트프로세스 컴포넌트가 있다면 하이라이트
	UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(OtherActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
	if (!!PostProcess)
		PostProcess->DrawRenderingRed();

	OverlappedActors.Add(OtherActor);
}

/** 홀딩 중 회전하는 동안 Skill Range가 함께 회전하며 Box에서 나가는 순간 
    PostProcess로 하이라이트 해제하면서 범위 밖으로 나갔는지 알려줌 */
void ACDoSkill_OneHand_02::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(OwnerCharacter, OtherActor));

	// #1. 범위 벗어난 Actor가 있다면 삭제할 index 저장
	int32 Index = OverlappedActors.Find(OtherActor);

	// #2. 삭제할 Index가 배열 안에 있다면 포스트프로세스 컴포넌트가 있는지 확인해서 하이라이트 해제시켜주기
	if (Index != INDEX_NONE)
	{
		UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(OtherActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
		if (!!PostProcess)
			PostProcess->ResetOutline();

		OverlappedActors.RemoveAt(Index);
	}
}

/** 스킬 활성화 되는 순간 Skill Range를 바닥에 찍기 ( 홀드하는 동안 ) */
void ACDoSkill_OneHand_02::Activate()
{
	Super::Activate();

	// #1. Skill Range 표시 , 범위 안에 있는 Actor들 Box로 탐색
	Decal->SetVisibility(true);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	// #2. Skill Range Decal을 찍기 위한 정보들 셋팅
	FVector up = CSub::GetCustomUpVector(this);
	up *= -90.f;
	FVector start = Box->GetComponentLocation();
	FVector end = start + up;

	TArray<TEnumAsByte<EObjectTypeQuery>> queries;
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	queries.Add(WorldStatic);

	TArray<AActor*> ignoreActors;

	// #3. Skill Range를 바닥에 셋팅가능하면 Decal 셋팅 ( 정적 ) -> 동적으로 하려면 Tick으로 옮겨야함
	FHitResult HitResult;
	if (UKismetSystemLibrary::LineTraceSingleForObjects(OwnerCharacter->GetWorld(), start, end, queries, false, ignoreActors, EDrawDebugTrace::None, HitResult, true))
	{
		Decal->SetWorldLocation(HitResult.ImpactPoint);
	}
}

/** 스킬이 비활성화 되는 순간 예외상황 처리를 위해 OverlappedActors들 초기화 시키기 */
void ACDoSkill_OneHand_02::Deactivate()
{
	Super::Deactivate();
	OverlappedActors.Empty();
}

/** 홀딩이 끝나면 회전을 멈추고 Skill Range에 있는 Actor들 중 가장 먼 거리의 액터까지로 이동 */
void ACDoSkill_OneHand_02::End_Hold()
{
	Super::End_Hold();

	// #1. 예외상황) Nocollison으로 바꾸는 순간 액터들이 모두 EndOverlap이 되어 OverlappedActors가 초기화 되는 
	//              예외가 있어서 미리 지역변수에 OverlappedActors를 저장하고 Nocollision, Skill Range 해제
	TArray<AActor*> SaveActors = OverlappedActors;
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Decal->SetVisibility(false);

	// #2. 홀드 해제하는 순간 겹쳐진 액터가 없다면 return
	if (!SaveActors.Num())
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection("EndMontage", Data.AnimMontage);
		return;
	}

	// #3. 겹쳐진 액터들 중 가장 거리가 먼 액터 가져오기
	AActor* FarthestActor = CSub::FindFarthestActor(OwnerCharacter, SaveActors);

	// #4. 가장 거리가 먼 액터 뒤로 이동하고나서 조금 더 가기 위해 일정 거리 구하기
	FVector distance = FarthestActor->GetActorLocation() - OwnerCharacter->GetActorLocation();
	FVector forward  = CSub::GetCustomForwardVector(OwnerCharacter);
	forward *= 300.f;
	FVector TargetRelativeLocation = distance + OwnerCharacter->GetActorLocation() + forward;


	// #5. 구한 거리까지 MoveComponentTo 하기 -> 0.22f
	FLatentActionInfo LatentInfo; // Latent == 숨어있는
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(OwnerCharacter->GetRootComponent(), TargetRelativeLocation, FRotator(0, 0, 0), false, true, 0.22f, false, EMoveComponentAction::Type::Move , LatentInfo);

	// #6. Skill Range에 있는 Actor들 모두 데미지 가하기
	for (AActor* DamageActor : SaveActors)
	{
		if(CSub::CustomTakeDamage(Data.Power , DamageActor, OwnerCharacter->GetController(), this))
		{
			if (!!HitEffect)
			{
				FTransform transfrom = FTransform(FRotator::ZeroRotator, DamageActor->GetActorLocation(), FVector(1.f));
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, transfrom, false, EPSCPoolMethod::AutoRelease);
			}

			if (!!HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, HitSound, DamageActor->GetActorLocation());
			}
		}

		// #7. 포스트 프로세스 컴포넌트 있다면 하이라이트 모두 해제
		UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(DamageActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
		PostProcess->ResetOutline();
	}
}

