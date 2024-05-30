#include "Action/DoSkill/TwoHandSkill/CDoSkill_TwoHand_03.h"
#include "Global.h"
//////////////////////////
#include "Components/CStateComponent.h"
//////////////////////////
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"

ACDoSkill_TwoHand_03::ACDoSkill_TwoHand_03()
{
	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	
	Effect = CreateDefaultSubobject<UParticleSystemComponent>("Effect");
	Effect->SetupAttachment(Scene);

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(Effect);

	index = 2;
	coolTime = 22.f;
}

void ACDoSkill_TwoHand_03::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ACDoSkill_TwoHand_03::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACDoSkill_TwoHand_03::ActorEndOverlap);
}

void ACDoSkill_TwoHand_03::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/** SubSkill 사용되는 순간 ActorBeginOverlap 켜지면서 HitEffect 발생 , HitSound 발생과 함께 범위기 */
void ACDoSkill_TwoHand_03::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(OwnerCharacter, OtherActor));

	if (CSub::CustomTakeDamage(Data.Power, OtherActor, OwnerCharacter->GetController(), this))
	{
		if (!!HitEffect)
		{
			FTransform transfrom = FTransform(FRotator::ZeroRotator, OtherActor->GetActorLocation(), FVector(1.f));
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, transfrom, false, EPSCPoolMethod::AutoRelease);
		}

		if (!!HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, OtherActor->GetActorLocation());
		}
	}
}

void ACDoSkill_TwoHand_03::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

}

/** SubSkill 사용되는 순간 범위기 Effect 발생과 Effect 사운드와 함께 범위기 */
void ACDoSkill_TwoHand_03::DoSubSkill()
{
	// #1. Skill 스폰위치 설정
	FVector forward = CSub::GetCustomForwardVector(OwnerCharacter);
	forward *= 300.f;
	FVector NewLocation = OwnerCharacter->GetActorLocation() + forward;
	SetActorLocation(NewLocation);


	// #2. 이펙트 생성 후 Collision 켭치게 만들어주고 일정시간뒤에 끄고 Effect 없애기
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (!!Effect)
	{
		Effect->Activate();
		FTimerHandle Delay;
		float DelayTime = 0.5f; // 값 설정
		GetWorld()->GetTimerManager().SetTimer(Delay, FTimerDelegate::CreateLambda([&]()
			{
				Effect->Deactivate();
				Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				GetWorld()->GetTimerManager().ClearTimer(Delay);
			}), DelayTime, false);    // 반복하려면 false를 true로 변경
	}

}

void ACDoSkill_TwoHand_03::Activate()
{
	Super::Activate();
}

void ACDoSkill_TwoHand_03::Deactivate()
{
	Super::Deactivate();
}



