#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActor_Sub_Star.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACActor_Sub_Star : public AActor
{
	GENERATED_BODY()
	
public:	
	ACActor_Sub_Star();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

#pragma region UPROPERTIES
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UParticleSystemComponent* Trail;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UParticleSystemComponent* Effect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USoundCue* ExplosionSound;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Damage;
#pragma endregion
};
