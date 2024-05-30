#include "Action/DoSkill/OneHandSkill/CActor_Sub_Star.h"
//////////////////////
#include "Global.h"
#include "Components/CStateComponent.h"
//////////////////////
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

ACActor_Sub_Star::ACActor_Sub_Star()
{
	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	Mesh  = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(Scene);

	Trail = CreateDefaultSubobject<UParticleSystemComponent>("Trail");
	Trail->SetupAttachment(Mesh);

	Effect = CreateDefaultSubobject<UParticleSystemComponent>("Effect");
	Effect->SetupAttachment(Mesh);

	Mesh->SetRelativeRotation(FRotator(0, -90, 90));

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");
}

void ACActor_Sub_Star::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ACActor_Sub_Star::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACActor_Sub_Star::ActorEndOverlap);
}

/** 표창이 OtherActor과 부딪히는 순간 Projectile을 멈추고 투명처리 , 이펙트 처리 , Destroy */
void ACActor_Sub_Star::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(GetOwner(), OtherActor));

	// #1. Dodge 제외 데미지 주기
	if (CSub::CustomTakeDamage(Damage, OtherActor, GetOwner()->GetInstigatorController(), this))
	{
		// #2. 멈추고 투명처리
		Projectile->StopMovementImmediately();
		Mesh->SetVisibility(false);


		// #3. Effect & Sound
		Effect->SetActive(true);
		if (!!ExplosionSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, ExplosionSound, OtherActor->GetActorLocation());
		}

		// #4. 0.25f초 뒤에 Destroy
		FTimerHandle Delay;
		float DelayTime = 0.25f;
		GetWorld()->GetTimerManager().SetTimer(Delay, FTimerDelegate::CreateLambda([&]()
			{
				// 코드 구현
				Destroy();
				// TimerHandle 초기화
				GetWorld()->GetTimerManager().ClearTimer(Delay);
			}), DelayTime, false);	// 반복하려면 false를 true로 변경
	}
}

void ACActor_Sub_Star::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
}

