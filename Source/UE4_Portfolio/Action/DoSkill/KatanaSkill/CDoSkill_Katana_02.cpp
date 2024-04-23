#include "Action/DoSkill/KatanaSkill/CDoSkill_Katana_02.h"
#include "Global.h"

#include "Components/CStatusComponent.h"
#include "Components/BoxComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Components/DecalComponent.h"



ACDoSkill_Katana_02::ACDoSkill_Katana_02()
{ 
	PrimaryActorTick.bCanEverTick = true; 

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
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

void ACDoSkill_Katana_02::Activate()
{
	Super::Activate();

	Decal->SetVisibility(true);

	//Box->Location
	FVector start = Box->GetComponentLocation();
	FVector end;

	FVector up = CSub::GetCustomUpVector(this);
	up *= -90.f;

	end = start + up;

	TArray<TEnumAsByte<EObjectTypeQuery>> queries;
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	queries.Add(WorldStatic);

	TArray<AActor*> ignoreActors;

	FHitResult HitResult;
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

void ACDoSkill_Katana_02::End_Hold()
{
	Super::End_Hold();
	bCanRotate = false;
	Decal->SetVisibility(false);
}

void ACDoSkill_Katana_02::DoSubSkill()
{
	Niagara->Activate(true);

	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


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
	CSub::CheckActorBeginOverlapped(OwnerCharacter, OtherActor);



	FDamageEvent e;
	OtherActor->TakeDamage(Data.Power, e, GetOwner()->GetInstigatorController(), this);
}

void ACDoSkill_Katana_02::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

}