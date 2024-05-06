#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActor_Sub_Slash.generated.h"

UCLASS()
class UE4_PORTFOLIO_API ACActor_Sub_Slash : public AActor
{
	GENERATED_BODY()
public:	
	ACActor_Sub_Slash();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

#pragma region SlashSetting
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UNiagaraComponent* Niagara;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UBoxComponent* Box;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UParticleSystem* Explosion;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USoundCue* ExplosionSound;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Damage;
#pragma endregion

};
