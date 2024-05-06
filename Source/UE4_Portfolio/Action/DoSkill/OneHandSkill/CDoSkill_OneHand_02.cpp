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
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(Scene);

	Decal = CreateDefaultSubobject<UDecalComponent>("Decal");
	Decal->SetupAttachment(Scene);

	index = 1;
}


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

void ACDoSkill_OneHand_02::Activate()
{
	Super::Activate();

	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

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

void ACDoSkill_OneHand_02::Deactivate()
{
	Super::Deactivate();

	OverlappedActors.Empty();
}

void ACDoSkill_OneHand_02::End_Hold()
{
	Super::End_Hold();

	TArray<AActor*> saveActors = OverlappedActors;
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Decal->SetVisibility(false);

	if (!saveActors.Num())
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection("EndMontage", Data.AnimMontage);
		return;
	}

	AActor* FarthestActor = CSub::FindFarthestActor(OwnerCharacter, saveActors);

	FVector distance = FarthestActor->GetActorLocation() - OwnerCharacter->GetActorLocation();

	FVector forward = CSub::GetCustomForwardVector(OwnerCharacter);
	forward *= 300.f;

	FVector TargetRelativeLocation = distance + OwnerCharacter->GetActorLocation() + forward;


	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(OwnerCharacter->GetRootComponent(), TargetRelativeLocation, FRotator(0, 0, 0), false, true, 0.22f, false, EMoveComponentAction::Type::Move , LatentInfo);

	for (AActor* actor : saveActors)
	{
		FDamageEvent e;
		UCStateComponent* ActorState = Cast<UCStateComponent>(actor->GetComponentByClass(UCStateComponent::StaticClass()));

		if (!!ActorState)
		{
			if (!ActorState->IsDodgeMode())
			{
				if (!!HitEffect)
				{
					FTransform transfrom = FTransform(FRotator::ZeroRotator, actor->GetActorLocation() , FVector(1.f));
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, transfrom, false, EPSCPoolMethod::AutoRelease);
				}

				if (!!HitSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, HitSound, actor->GetActorLocation());
				}

				actor->TakeDamage(Data.Power, e, OwnerCharacter->GetController(), this);

			}
		}
		

		UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(actor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
		PostProcess->ResetOutline();
	}
}

void ACDoSkill_OneHand_02::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	CSub::CheckActorBeginOverlapped(OwnerCharacter,OtherActor);

	UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(OtherActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
	PostProcess->DrawRenderingRed();

	OverlappedActors.Add(OtherActor);
}

void ACDoSkill_OneHand_02::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	CSub::CheckActorBeginOverlapped(OwnerCharacter, OtherActor);

	int32 Index = OverlappedActors.Find(OtherActor);

	if (Index != INDEX_NONE)
	{
		UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(OtherActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
		PostProcess->ResetOutline();

		OverlappedActors.RemoveAt(Index);
	}
}

