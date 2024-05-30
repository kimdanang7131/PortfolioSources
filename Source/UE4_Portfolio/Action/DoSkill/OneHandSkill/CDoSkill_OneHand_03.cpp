#include "Action/DoSkill/OneHandSkill/CDoSkill_OneHand_03.h"
#include "Global.h"
/////////////////////////
#include "Components/CPostProcessComponent.h"
#include "Components/CStateComponent.h"
/////////////////////////
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"

ACDoSkill_OneHand_03::ACDoSkill_OneHand_03()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	Box   = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(Scene);

	Decal = CreateDefaultSubobject<UDecalComponent>("Decal");
	Decal->SetupAttachment(Scene);

	index = 2;
	coolTime = 19.f;
}

/** 스킬 시전 중 회전가능하도록 설정 */
void ACDoSkill_OneHand_03::BeginPlay()
{
	Super::BeginPlay();
	bCanRotate = true;

	OnActorBeginOverlap.AddDynamic(this, &ACDoSkill_OneHand_03::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACDoSkill_OneHand_03::ActorEndOverlap);
}

void ACDoSkill_OneHand_03::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/** 겹쳐진 Actor들이 들어올 때마다 가장 가까운 거리의 액터 찾아서 포스트 프로세스 하이라이트 시키기 */
void ACDoSkill_OneHand_03::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(OwnerCharacter, OtherActor));

	OverlappedActors.Add(OtherActor);
	/*int32 index = OverlappedActors.Find(OtherActor);*/

	// #1. 새롭게 겹쳐진 Actor + 현재 겹쳐진 Actors중 가장 가까운 Actor 찾기
	AActor* TempActor = CSub::FindNearestActor(OwnerCharacter, OverlappedActors);

	// #2. TargetActor( 하이라이트 된 Actor ) 와 새롭게 겹쳐진 액터와 다를때 -> TargetActor( 하이라이트 된 Actor ) 하이라이트 해제 먼저 시키기
	if (TempActor != TargetActor && OverlappedActors.Num() > 0)
	{
		if (TargetActor != nullptr)
		{
			UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(TargetActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
			if (!!PostProcess)
				PostProcess->ResetOutline();
		}
	}

	// #3. 위에서 하이라이트 해제 작업 끝났으니 TargetActor 지정 후 하이라이트
	TargetActor = TempActor;

	UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(TargetActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
	if (!!PostProcess)
		PostProcess->DrawRenderingRed();
}

/** 겹쳐진 Actor들이 나갈 때마다 하이라이트 제거 후 가장 가까운 거리의 액터 찾아서 포스트 프로세스 하이라이트 시키기 */
void ACDoSkill_OneHand_03::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(OwnerCharacter, OtherActor));

	// #1. 나간 Actor Index 찾기
	int32 fIndex = OverlappedActors.Find(OtherActor);

	// #2. OtherActor가 배열 안에 있다면 진행
	if (fIndex != INDEX_NONE)
	{
		// #3. 일단 Target이든 아니든 포스트 프로세스 하이라이트 해제
		UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(OtherActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
		if(!!PostProcess)
			PostProcess->ResetOutline();

		// #4. 나간 Actor가 TargetActor ( 하이라이트 된 Actor ) 같다면 TargetActor 비우기  
		if (OtherActor == TargetActor)
			TargetActor = nullptr;

		OverlappedActors.RemoveAt(fIndex);

		// #5. 지웠는데도 SkillRange 안에 액터들이 남아있다면 새로 가까운 액터 찾아서 하이라이트
		if (OverlappedActors.Num() > 0)
		{
			AActor* TempActor = CSub::FindNearestActor(OwnerCharacter, OverlappedActors);

			if (!!TempActor)
			{
				TargetActor = TempActor;

				UCPostprocessComponent* TargetPostProcess = Cast<UCPostprocessComponent>(TargetActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
				if(!!TargetPostProcess)
					TargetPostProcess->DrawRenderingRed();
			}
		}
	}
}

/** 스킬 활성화 되는 순간 Skill Range를 바닥에 찍기 ( 홀드하는 동안 ) */
void ACDoSkill_OneHand_03::Activate()
{
	Super::Activate();
	bCanRotate = true;

	// #1. Skill Range 표시 , 범위 안에 있는 Actor들 Box로 탐색
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Decal->SetVisibility(true);

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
void ACDoSkill_OneHand_03::Deactivate()
{
	Super::Deactivate();
	OverlappedActors.Empty();
}

/** 홀딩이 끝나면 회전을 멈추고 Skill Range에 있는 Actor들 중 가장 가까운 거리의 액터로 이동 */
void ACDoSkill_OneHand_03::End_Hold()
{
	bCanRotate = false;

	// #1. 예외상황) Nocollison으로 바꾸는 순간 액터들이 모두 EndOverlap이 되어 OverlappedActors가 초기화 되는 
    //              예외가 있어서 미리 지역변수에 OverlappedActors를 저장하고 Nocollision, Skill Range 해제
	AActor* TargetTemp = TargetActor;
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Decal->SetVisibility(false);

	// #2. 홀드 해제하는 순간 타겟이 없다면 return
	if (TargetTemp == nullptr)
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection("EndMontage", Data.AnimMontage);
		return;
	}

	// #3. 타겟 뒤로 이동시킨 후 타겟 쳐다보기 
	FVector TargetLocation = TargetTemp->GetActorLocation();
	FVector TargetForward  = CSub::GetCustomForwardVector(TargetTemp);
	FRotator NewRotation   = TargetTemp->GetActorRotation();
	TargetForward *= -100.f;

	FVector NewLocation = TargetLocation + TargetForward;
	OwnerCharacter->SetActorLocationAndRotation(NewLocation, NewRotation);

	// #4. 이동하고나서 애니메이션 실행해야 하므로
	Super::End_Hold();


	// #5. 타겟이 포스트프로세스 컴포넌트 있다면 하이라이트 해제시켜주기
	UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(TargetTemp->GetComponentByClass(UCPostprocessComponent::StaticClass()));
	PostProcess->ResetOutline();

	TargetTemp  = nullptr;
	TargetActor = nullptr;
}


