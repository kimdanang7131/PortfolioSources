#include "Action/DoSkill/OneHandSkill/CActor_Sub_Star.h"

#include "Particles/ParticleSystemComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Components/CStateComponent.h"

ACActor_Sub_Star::ACActor_Sub_Star()
{
	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
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


void ACActor_Sub_Star::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(GetOwner(), OtherActor));

	//ACharacter* character = Cast<ACharacter>(OtherActor);
	//NULL_RETURN(character);
	// ApplyDamage ( TakeDamage )
	FDamageEvent e;

	UCStateComponent* TargetState = Cast<UCStateComponent>(OtherActor->GetComponentByClass(UCStateComponent::StaticClass()));

	if (!TargetState->IsDodgeMode())
		OtherActor->TakeDamage(Damage, e, GetOwner()->GetInstigatorController(), this);



	Projectile->StopMovementImmediately();
	Effect->SetActive(true);
	Mesh->SetVisibility(false);

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

void ACActor_Sub_Star::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
}

