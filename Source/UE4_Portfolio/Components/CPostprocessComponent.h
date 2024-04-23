#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPostprocessComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCPostprocessComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPostprocessComponent();

protected:
	virtual void BeginPlay() override;


public:
	void DrawOutlineRed();
	void DrawOutlineBlue();
	void DrawOutlineGreen();
	void DrawOutline(const int32& value);
	void ResetOutline();

	void DrawRenderingRed();
	void DrawRenderingBlue();
	void DrawRenderingGreen();



protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USkeletalMeshComponent* OwnerMesh;

	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class UMaterialParameterCollection* collection;
	class UMaterialParameterCollectionInstance* MPC;
};
