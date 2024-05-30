#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPostprocessComponent.generated.h"

/** ParameterCollection을 통해 실시간으로 포스트 프로세스 조정 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCPostprocessComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UCPostprocessComponent();
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USkeletalMeshComponent* OwnerMesh;

	/** Outline 사용시 MPC Outline 값을 1 -> Outline 안하면 0으로 */
	/** Outline 조정 5 , 6 , 7 */
public:
#pragma region Outline
	void DrawOutlineRed();
	void DrawOutlineBlue();
	void DrawOutlineGreen();

	void DrawOutline(const int32& value);
	void ResetOutline();
#pragma endregion

	/** 채도제거 하려면 MPC Desaturate 값을 1 -> 본색깔 유지하려면 MPC Desaturation 값을 0 */
	/** Color Rendering 조정 2, 3, 4 */
#pragma region ColorRendering
	void DrawRenderingRed();
	void DrawRenderingBlue();
	void DrawRenderingGreen();
	
	void DrawRenderingDesaturate();
	void ResetColorRendering();
#pragma endregion

private:
	class UMaterialParameterCollectionInstance* MPC;
	class UMaterialParameterCollection* ParameterCollection;
};
