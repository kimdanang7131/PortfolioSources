#include "Action/DoSkill/TwoHandSkill/CDoSkill_TwoHand_03.h"
#include "Global.h"

#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"

ACDoSkill_TwoHand_03::ACDoSkill_TwoHand_03()
{
	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	
	Effect = CreateDefaultSubobject<UParticleSystemComponent>("Effect");
	Effect->SetupAttachment(Scene);

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(Effect);

	index = 2;
}

void ACDoSkill_TwoHand_03::BeginPlay()
{
	Super::BeginPlay();
	bCanRotate = true;
	rInterpSpeed = 3.f;

	OnActorBeginOverlap.AddDynamic(this, &ACDoSkill_TwoHand_03::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACDoSkill_TwoHand_03::ActorEndOverlap);

}

void ACDoSkill_TwoHand_03::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACDoSkill_TwoHand_03::Activate()
{
	Super::Activate();
}

void ACDoSkill_TwoHand_03::Deactivate()
{
	Super::Deactivate();
}

void ACDoSkill_TwoHand_03::DoSubSkill()
{
	FVector OwnerLocation = OwnerCharacter->GetActorLocation();
	FVector forward = CSub::GetCustomForwardVector(OwnerCharacter);

	forward *= 300.f;

	FVector NewLocation = OwnerLocation + forward;
	SetActorLocation(NewLocation);

	Effect->Activate();
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	FTimerHandle Delay;
	float DelayTime = 0.5f; // 값 설정
	GetWorld()->GetTimerManager().SetTimer(Delay, FTimerDelegate::CreateLambda([&]()
		{
			Effect->Deactivate();
			Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			GetWorld()->GetTimerManager().ClearTimer(Delay);
		}), DelayTime, false);    // 반복하려면 false를 true로 변경
}


void ACDoSkill_TwoHand_03::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(OwnerCharacter, OtherActor));

	FDamageEvent e;
	OtherActor->TakeDamage(Data.Power, e, OwnerCharacter->GetController(), this);
}

void ACDoSkill_TwoHand_03::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor) 
{

}