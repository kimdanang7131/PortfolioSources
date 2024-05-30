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

/** SubSkill ���Ǵ� ���� ActorBeginOverlap �����鼭 HitEffect �߻� , HitSound �߻��� �Բ� ������ */
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

/** SubSkill ���Ǵ� ���� ������ Effect �߻��� Effect ����� �Բ� ������ */
void ACDoSkill_TwoHand_03::DoSubSkill()
{
	// #1. Skill ������ġ ����
	FVector forward = CSub::GetCustomForwardVector(OwnerCharacter);
	forward *= 300.f;
	FVector NewLocation = OwnerCharacter->GetActorLocation() + forward;
	SetActorLocation(NewLocation);


	// #2. ����Ʈ ���� �� Collision ��ġ�� ������ְ� �����ð��ڿ� ���� Effect ���ֱ�
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (!!Effect)
	{
		Effect->Activate();
		FTimerHandle Delay;
		float DelayTime = 0.5f; // �� ����
		GetWorld()->GetTimerManager().SetTimer(Delay, FTimerDelegate::CreateLambda([&]()
			{
				Effect->Deactivate();
				Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				GetWorld()->GetTimerManager().ClearTimer(Delay);
			}), DelayTime, false);    // �ݺ��Ϸ��� false�� true�� ����
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



