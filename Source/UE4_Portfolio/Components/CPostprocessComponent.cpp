#include "Components/CPostprocessComponent.h"
#include "Global.h"
////////////////////////////
#include "GameFramework/Character.h"
////////////////////////////
#include "Runtime/Engine/Classes/Materials/MaterialParameterCollectionInstance.h"
#include "Materials/MaterialParameterCollection.h"

#include "Components/SkeletalMeshComponent.h"

UCPostprocessComponent::UCPostprocessComponent()
{
	CHelpers::MyFObjectFinder<UMaterialParameterCollection>(&ParameterCollection, TEXT("MaterialParameterCollection'/Game/Materials/MPC_Data.MPC_Data'"));
}

void UCPostprocessComponent::BeginPlay()
{
	Super::BeginPlay();

	// #1. ParameterCollection�� Instance�� ���� �� �װ� MPC �����Ϳ� �־��
	MPC = GetWorld()->GetParameterCollectionInstance(ParameterCollection);

	// #2. OwnerCharacter , Mesh ��� , CustomDepth ���� �� �ֵ��� ����
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!!OwnerCharacter)
		OwnerMesh = OwnerCharacter->GetMesh();
	if (!!OwnerMesh)
		OwnerMesh->SetRenderCustomDepth(true);

	// #3. �⺻���� ( 0 )���� �ʱ� ����
	OwnerMesh->SetCustomDepthStencilValue(0);
}

/** Outline ���� 5 , 6 , 7 */
#pragma region Outline
void UCPostprocessComponent::DrawOutlineRed()
{
	DrawOutline(5);
}

void UCPostprocessComponent::DrawOutlineBlue()
{
	DrawOutline(6);
}

void UCPostprocessComponent::DrawOutlineGreen()
{
	DrawOutline(7);
}

/** Outline ����Ϸ��� MPC Outline�� 1 -> Outline�� ������ MPC Outline�� 0���� */
void UCPostprocessComponent::DrawOutline(const int32& value)
{
	MPC->SetScalarParameterValue("Outline", 1);
	OwnerMesh->SetCustomDepthStencilValue(value);
}

/** Outline*/
void UCPostprocessComponent::ResetOutline()
{
	MPC->SetScalarParameterValue("Outline", 0);
	OwnerMesh->SetCustomDepthStencilValue(0);
}

#pragma endregion

/** Color Rendering ���� 2 , 3 , 4 */
#pragma region ColorRendering
void UCPostprocessComponent::DrawRenderingRed()
{
	OwnerMesh->SetCustomDepthStencilValue(2);
}

void UCPostprocessComponent::DrawRenderingBlue()
{
	OwnerMesh->SetCustomDepthStencilValue(3);
}

void UCPostprocessComponent::DrawRenderingGreen()
{
	OwnerMesh->SetCustomDepthStencilValue(4);
}

void UCPostprocessComponent::DrawRenderingDesaturate()
{
	MPC->SetScalarParameterValue("Desaturate", 1);
}

void UCPostprocessComponent::ResetColorRendering()
{
	MPC->SetScalarParameterValue("Desaturate", 0);
	OwnerMesh->SetCustomDepthStencilValue(0);
}
#pragma endregion
