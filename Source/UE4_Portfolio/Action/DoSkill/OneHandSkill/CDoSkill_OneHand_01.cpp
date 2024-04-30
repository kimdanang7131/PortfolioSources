#include "Action/DoSkill/OneHandSkill/CDoSkill_OneHand_01.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Action/DoSkill/OneHandSkill/CActor_Sub_Star.h"
#include "Kismet/KismetMathLibrary.h"


ACDoSkill_OneHand_01::ACDoSkill_OneHand_01()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::MyFClassFinder<ACActor_Sub_Star>(&starClass, "Blueprint'/Game/Blueprints/DataAsset/Actors/SkillSubActors/BP_CActor_Sub_Star.BP_CActor_Sub_Star_C'");
	//ConstructorHelpers::MyFClassFinder<ACActor_Sub_Star>(TEXT("Blueprint'/Game/Blueprints/DataAsset/SkillSubActors/BP_CActor_Sub_Star.BP_CActor_Sub_Star_C'"));

	index = 0;
}
void ACDoSkill_OneHand_01::BeginPlay()
{
	Super::BeginPlay();

	bCanRotate = true;

}

void ACDoSkill_OneHand_01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACDoSkill_OneHand_01::Activate()
{
	Super::Activate();
}

void ACDoSkill_OneHand_01::Deactivate()
{
	Super::Deactivate();
}



void ACDoSkill_OneHand_01::DoSubSkill()
{
	// GetAssetDynmic�� ���� 
	//UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Blueprints/DataAsset/SkillSubActors/BP_OneHand_Sub_Star.BP_OneHand_Sub_Star'")));
	
	if (starCount < 3)
	{
		// #1. ĳ���� �߽� ���� ������ �������� ǥâ ��ȯ
		for (int i = -1; i <= 1; i++)
		{
			// #2. ������ ���������� Blueprint �����ͼ� ��ȯ 
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = GetOwner();
			ACActor_Sub_Star* star = CHelpers::MySpawnActor<ACActor_Sub_Star>(starClass, GetOwner(), GetOwner()->GetTransform());

			//ACActor_Sub_Star* star = GetWorld()->SpawnActor<ACActor_Sub_Star>(starClass, GetOwner()->GetTransform(), SpawnParameters);
			// #3. Owner�� ������ġ���� ������ ������ �ΰ� ��ȯ�Ҷ� �ʿ��� ����
			FVector ownerLocation   = OwnerCharacter->GetActorLocation();
			const FRotator& rotator = OwnerCharacter->GetActorRotation();

			FVector rightVector = FQuat(rotator).GetRightVector();

			// #4. ������ �������� ������ enum ������ �а���(enum hack)�� �̿��Ͽ� ��ġ �����ϱ�
			float interval = (float)INTERVAL * i;
			rightVector *= interval;
;			FVector ResultLocation = ownerLocation + rightVector;

			star->SetActorLocation(ResultLocation);
		}
		starCount++;
		return;
	}

	// #1. ĳ���� �߽� ���� ������ �������� ǥâ ��ȯ
	for (int i = -2; i <= 2; i++)
	{
		// #2. ������ ���������� Blueprint �����ͼ� ��ȯ 
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = GetOwner();
		ACActor_Sub_Star* star = CHelpers::MySpawnActor<ACActor_Sub_Star>(starClass, GetOwner(), GetOwner()->GetTransform());
		//ACActor_Sub_Star* star = GetWorld()->SpawnActor<ACActor_Sub_Star>(starClass, GetOwner()->GetTransform(), SpawnParameters);

		// #3. Owner�� ������ġ���� ������ ������ �ΰ� ��ȯ�Ҷ� �ʿ��� ����
		FVector ownerLocation = OwnerCharacter->GetActorLocation();
		const FRotator& rotator = OwnerCharacter->GetActorRotation();
		FVector rightVector = FQuat(rotator).GetRightVector();

		// #4. ������ �������� ������ enum ������ �а���(enum hack)�� �̿��Ͽ� ��ġ �����ϱ�
		float interval = (float)INTERVAL * i;
		rightVector *= interval;
		FVector ResultLocation = ownerLocation + rightVector;

		star->SetActorLocation(ResultLocation);
	}

	starCount = 1;
}



//// #1. ĳ���� �߽� ���� ������ �������� ǥâ ��ȯ
//for (int i = -1; i <= 1; i++)
//{
//	// #2. ������ ���������� Blueprint �����ͼ� ��ȯ 
//	AActor* star = CHelpers::SpawnActorFromBPObject<AActor>(SpawnActor, GetOwner());
//
//	// #3. Owner�� ������ġ���� ������ ������ �ΰ� ��ȯ�Ҷ� �ʿ��� ����
//	FVector ownerLocation = OwnerCharacter->GetActorLocation();
//	const FRotator& rotator = OwnerCharacter->GetActorRotation();
//	FVector rightVector = FQuat(rotator).GetRightVector();
//
//	// #4. ������ �������� ������ enum ������ �а���(enum hack)�� �̿��Ͽ� ��ġ �����ϱ�
//	float interval = (float)INTERVAL * i;
//	rightVector *= interval;
//	FVector ResultLocation = ownerLocation + rightVector;
//
//	star->SetActorLocation(ResultLocation);
//
//	TArray<class UShapeComponent*> colliders;
//	star->GetComponents<UShapeComponent>(colliders);
//
//	for (UShapeComponent* component : colliders)
//	{
//		component->OnComponentBeginOverlap.AddDynamic(OwnerWeapon, &ACWeapon::OnComponentBeginOverlap);
//		component->OnComponentEndOverlap.AddDynamic(OwnerWeapon, &ACWeapon::OnComponentEndOverlap);
//	}
//}