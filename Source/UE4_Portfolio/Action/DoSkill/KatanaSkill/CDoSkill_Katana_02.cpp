#include "Action/DoSkill/KatanaSkill/CDoSkill_Katana_02.h"
#include "Global.h"
/////////////////////////////
#include "Components/CStatusComponent.h"
/////////////////////////////
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"

ACDoSkill_Katana_02::ACDoSkill_Katana_02()
{ 
	PrimaryActorTick.bCanEverTick = true; 

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	Box   = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(Scene);

	Decal = CreateDefaultSubobject<UDecalComponent>("SkillRange");
	Decal->SetupAttachment(Scene);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>("NPS_Stab");
	Niagara->SetupAttachment(Scene);

	index = 1;
}

void ACDoSkill_Katana_02::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ACDoSkill_Katana_02::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACDoSkill_Katana_02::ActorEndOverlap);
}

void ACDoSkill_Katana_02::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/** 스킬 활성화 시 바닥에 SkillRange Deacl 표시 후 그 범위에 이펙트 + 공격 이후 막타 검기 -> 검기는 Notify를 통해 */
void ACDoSkill_Katana_02::Activate()
{
	Super::Activate();

	Decal->SetVisibility(true);
	FVector upVector = CSub::GetCustomUpVector(this);
	upVector *= -90.f;

	// #1. 박스 아래로 Trace 쏘기 위한 설정들
	FVector start = Box->GetComponentLocation();
	FVector end   = start + upVector;

	// #2.ECollisionChannel에 바닥에 대한 Enum을 ObjectType으로 변환하여 Query에 추가해줌
	TArray<TEnumAsByte<EObjectTypeQuery>> queries;
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	queries.Add(WorldStatic);

	TArray<AActor*> ignoreActors;
	FHitResult HitResult;

	// #3. 박스 아래로 Trace를 쏘아 바닥(WorldStatic)이라면 SkillRange( Decal ) 표시
	if (UKismetSystemLibrary::LineTraceSingleForObjects(OwnerCharacter->GetWorld(), start, end, queries, false, ignoreActors, EDrawDebugTrace::None, HitResult, true))
	{
		Decal->SetWorldLocation(HitResult.ImpactPoint);
	}
}

void ACDoSkill_Katana_02::Deactivate()
{
	Super::Deactivate();
	bCanRotate = true;
}

/** 홀드를 끝내는 순간 회전 못하게 하고 SkillRange 안보이도록 설정 */
void ACDoSkill_Katana_02::End_Hold()
{
	Super::End_Hold();
	bCanRotate = false;
	Decal->SetVisibility(false);
}

/**  */
void ACDoSkill_Katana_02::DoSubSkill()
{
	// #1. Stab Effect 보여주고 데미지 가할 수 있도록 Collision 변경
	Niagara->Activate(true);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// #2. 간단 타이머 람다로 구현하여 Effect 잠시 나오게 한후 Collision 깜빡깜빡하여 데미지 넣도록
	FTimerHandle Delay;
	float DelayTime = 0.1f;
	GetWorld()->GetTimerManager().SetTimer(Delay, FTimerDelegate::CreateLambda([&]()
		{
			Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			// TimerHandle 초기화
			GetWorld()->GetTimerManager().ClearTimer(Delay);
		}), DelayTime, false);	// 반복하려면 false를 true로 변경
}

void ACDoSkill_Katana_02::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(OwnerCharacter, OtherActor));

	CSub::CustomTakeDamage(Data.Power, OtherActor, GetOwner()->GetInstigatorController(), this);
}

void ACDoSkill_Katana_02::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

}