#include "Action/DoSkill/OneHandSkill/CDoSkill_OneHand_01.h"
#include "Global.h"
/////////////////////
#include "Action/DoSkill/OneHandSkill/CActor_Sub_Star.h"
/////////////////////
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

ACDoSkill_OneHand_01::ACDoSkill_OneHand_01()
{
	PrimaryActorTick.bCanEverTick = true;
	index = 0;
	coolTime = 11.f;
}

/** ��ų ����� ȸ�� ���ɵ��� ���� */
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

/** Notify�� ���Ͽ� ǥâ ������ Ÿ�̹� ���� �� �� ��ų ���� */
void ACDoSkill_OneHand_01::DoSubSkill()
{
	// #1. 1 , 2������ if�� ������ �����Ϸ��� -> 3 3 5�� �������� 1�� �Է��ؾ���
	if (starCount < 3)
	{
		SpawnStars(starCount);

		// #2. 1�� -> 2������ �����ϴٰ� 3��° �Ʒ��� if���� ����� �Ʒ� �������� �Ѿ
		starCount++;
		return;
	}

	// #3. starCount�� [ 3�� ] °�� ����
	SpawnStars(starCount);
	starCount = 1;
}

/** ���� ǥâ������ŭ ���� �������� ǥâ ��ȯ�ؼ� ������ */
void ACDoSkill_OneHand_01::SpawnStars(const int32& InStarCount)
{
	// #1. ĳ���� �߽� ���� ������ �������� ǥâ  1, 3�� ��ȯ
	for (int i = -InStarCount; i <= InStarCount; i++)
	{
		// #2. ����� ǥâ Ŭ�����κ��� SpawnActor
		ACActor_Sub_Star* star = CHelpers::MySpawnActor<ACActor_Sub_Star>(starClass, GetOwner(), GetOwner()->GetTransform());
		//ACActor_Sub_Star* star = GetWorld()->SpawnActor<ACActor_Sub_Star>(starClass, GetOwner()->GetTransform(), SpawnParameters);

		// #3. Owner�� ������ġ���� ������ ������ �ΰ� ��ȯ�Ҷ� �ʿ��� ����
		const FVector& ownerLocation = OwnerCharacter->GetActorLocation();
		const FRotator& rotator = OwnerCharacter->GetActorRotation();
		FVector rightVector = FQuat(rotator).GetRightVector();

		// #4. ������ �������� ������ enum ������ �а���(enum hack)�� �̿��Ͽ� ��ġ �����ϱ�
		float interval = static_cast<float>(INTERVAL) * i;
		rightVector *= interval;
		FVector ResultLocation = ownerLocation + rightVector;

		star->SetActorLocation(ResultLocation);
	}
}


