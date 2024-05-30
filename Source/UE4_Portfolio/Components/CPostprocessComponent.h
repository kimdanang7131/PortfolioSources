#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPostprocessComponent.generated.h"

/** ParameterCollection�� ���� �ǽð����� ����Ʈ ���μ��� ���� */
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

	/** Outline ���� MPC Outline ���� 1 -> Outline ���ϸ� 0���� */
	/** Outline ���� 5 , 6 , 7 */
public:
#pragma region Outline
	void DrawOutlineRed();
	void DrawOutlineBlue();
	void DrawOutlineGreen();

	void DrawOutline(const int32& value);
	void ResetOutline();
#pragma endregion

	/** ä������ �Ϸ��� MPC Desaturate ���� 1 -> ������ �����Ϸ��� MPC Desaturation ���� 0 */
	/** Color Rendering ���� 2, 3, 4 */
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
