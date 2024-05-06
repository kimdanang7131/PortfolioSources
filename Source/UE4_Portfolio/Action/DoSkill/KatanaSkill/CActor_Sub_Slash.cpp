#include "Action/DoSkill/KatanaSkill/CActor_Sub_Slash.h"
#include "Global.h"
///////////////////////
#include "Components/CStateComponent.h"
///////////////////////
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"
///////////////////////

ACActor_Sub_Slash::ACActor_Sub_Slash()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara", Scene);
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Niagara);
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	/** Projectile 설정 */
	Projectile->InitialSpeed = 3000.f;
	Projectile->MaxSpeed     = 3000.f;
	Projectile->ProjectileGravityScale = 0.f;
}

void ACActor_Sub_Slash::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(10.f);

	OnActorBeginOverlap.AddDynamic(this, &ACActor_Sub_Slash::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACActor_Sub_Slash::ActorEndOverlap);
}

void ACActor_Sub_Slash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/** Slash가 적에게 부딪혔을 때 Explosion , Sound 와 함께 사라지며 데미지 가함 */
void ACActor_Sub_Slash::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(GetOwner(), OtherActor));
	
	if (CSub::CustomTakeDamage(Damage, OtherActor, GetOwner()->GetInstigatorController(), this))
	{
		if (!!Explosion)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, OtherActor->GetTransform(), false, EPSCPoolMethod::AutoRelease);
		}
		if (!!ExplosionSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, ExplosionSound, OtherActor->GetActorLocation());
		}
	}
}

void ACActor_Sub_Slash::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

}