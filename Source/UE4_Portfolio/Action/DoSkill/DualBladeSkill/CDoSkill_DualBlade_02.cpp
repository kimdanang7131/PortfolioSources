#include "Action/DoSkill/DualBladeSkill/CDoSkill_DualBlade_02.h"
#include "Global.h"
////////////////////
#include "GameFramework/Character.h"

void ACDoSkill_DualBlade_02::BeginPlay()
{
	Super::BeginPlay();
}

void ACDoSkill_DualBlade_02::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// #1. ��ų�� Ȱ��ȭ �Ǹ� �����ð����� �ڷ� ���ٰ� Notify�� ���� 
	//     Converter�� �ݴ�� �ٲ�� ������ ����
	if (bIsActivated)
	{
		FRotator ownerRotator = OwnerCharacter->GetActorRotation();
		FVector forward = FQuat(ownerRotator).GetForwardVector();

		float newOffset = speed * converter;

		FVector DeltaLocation = forward * newOffset;
		OwnerCharacter->AddActorWorldOffset(DeltaLocation, true);
	}
}

/** ��ų Ȱ��ȭ �Ǹ� �ӵ� , ���� ���� */
void ACDoSkill_DualBlade_02::Activate()
{
	Super::Activate();

	converter = -1.f;
	speed     = 6.f;
}

void ACDoSkill_DualBlade_02::Deactivate()
{
	Super::Deactivate();
}

/** Notify�� ���Ͽ� �ӵ� , ���� �ٲٱ� */
void ACDoSkill_DualBlade_02::DoSubSkill()
{
	converter *= -1.f;
	speed      = 18.f;
}

