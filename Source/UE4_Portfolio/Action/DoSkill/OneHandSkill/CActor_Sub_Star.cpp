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

/** ǥâ�� OtherActor�� �ε����� ���� Projectile�� ���߰� ����ó�� , ����Ʈ ó�� , Destroy */
void ACActor_Sub_Star::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(GetOwner(), OtherActor));

	// #1. Dodge ���� ������ �ֱ�
	if (CSub::CustomTakeDamage(Damage, OtherActor, GetOwner()->GetInstigatorController(), this))
	{
		// #2. ���߰� ����ó��
		Projectile->StopMovementImmediately();
		Mesh->SetVisibility(false);


		// #3. Effect & Sound
		Effect->SetActive(true);
		if (!!ExplosionSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, ExplosionSound, OtherActor->GetActorLocation());
		}

		// #4. 0.25f�� �ڿ� Destroy
		FTimerHandle Delay;
		float DelayTime = 0.25f;
		GetWorld()->GetTimerManager().SetTimer(Delay, FTimerDelegate::CreateLambda([&]()
			{
				// �ڵ� ����
				Destroy();
				// TimerHandle �ʱ�ȭ
				GetWorld()->GetTimerManager().ClearTimer(Delay);
			}), DelayTime, false);	// �ݺ��Ϸ��� false�� true�� ����
	}
}

void ACActor_Sub_Star::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
}

