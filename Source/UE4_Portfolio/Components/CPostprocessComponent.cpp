#include "Components/CPostprocessComponent.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Rendering/RenderingCommon.h"
#include "Runtime/Engine/Classes/Materials/MaterialParameterCollectionInstance.h"

UCPostprocessComponent::UCPostprocessComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	UMaterialParameterCollection* mpcData;
	CHelpers::GetAsset<UMaterialParameterCollection>(&mpcData, TEXT("MaterialParameterCollection'/Game/Materials/MPC_Data.MPC_Data'"));
	collection = mpcData;
}


void UCPostprocessComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (!!OwnerCharacter)
		OwnerMesh = OwnerCharacter->GetMesh();

	if (!!OwnerMesh)
		OwnerMesh->SetRenderCustomDepth(true);

	MPC = GetWorld()->GetParameterCollectionInstance(collection);

	OwnerMesh->SetCustomDepthStencilValue(0);

}


//void UCPostprocessComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//}



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

void UCPostprocessComponent::DrawOutline(const int32& value)
{
	MPC->SetScalarParameterValue("Outline", value);
	OwnerMesh->SetCustomDepthStencilValue(value);
}

void UCPostprocessComponent::ResetOutline()
{
	MPC->SetScalarParameterValue("Outline", 0);
	OwnerMesh->SetCustomDepthStencilValue(0);
}




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