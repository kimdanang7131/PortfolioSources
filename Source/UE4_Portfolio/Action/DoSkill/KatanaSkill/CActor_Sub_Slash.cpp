#include "Action/DoSkill/KatanaSkill/CActor_Sub_Slash.h"

#include "Particles/ParticleSystemComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CStateComponent.h"


ACActor_Sub_Slash::ACActor_Sub_Slash()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara", Scene);
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Niagara);

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	CHelpers::GetAsset<UParticleSystem>(&Explosion, "ParticleSystem'/Game/Particles/P_Hit_Fire.P_Hit_Fire'");

	Projectile->InitialSpeed = 3000.f;
	Projectile->MaxSpeed = 3000.f;
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


void ACActor_Sub_Slash::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(GetOwner(), OtherActor));
	
	FDamageEvent e;

	UCStateComponent* TargetState = Cast<UCStateComponent>(OtherActor->GetComponentByClass(UCStateComponent::StaticClass()));

	if (!TargetState->IsDodgeMode())
		OtherActor->TakeDamage(Damage, e, GetOwner()->GetInstigatorController(), this);

	if (!!Explosion)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, OtherActor->GetTransform(), false,EPSCPoolMethod::AutoRelease );
	}
}

void ACActor_Sub_Slash::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

}