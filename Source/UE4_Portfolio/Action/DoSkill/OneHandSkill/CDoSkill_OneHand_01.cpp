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

/** 스킬 사용중 회전 가능도록 설정 */
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

/** Notify를 통하여 표창 날리는 타이밍 지정 후 이 스킬 실행 */
void ACDoSkill_OneHand_01::DoSubSkill()
{
	// #1. 1 , 2개까지 if문 내에서 진행하려면 -> 3 3 5로 날릴려면 1로 입력해야함
	if (starCount < 3)
	{
		SpawnStars(starCount);

		// #2. 1개 -> 2개까지 증가하다가 3개째 아래로 if문을 벗어나서 아래 로직으로 넘어감
		starCount++;
		return;
	}

	// #3. starCount가 [ 3개 ] 째에 들어옴
	SpawnStars(starCount);
	starCount = 1;
}

/** 들어온 표창개수만큼 일정 간격으로 표창 소환해서 날리기 */
void ACDoSkill_OneHand_01::SpawnStars(const int32& InStarCount)
{
	// #1. 캐릭터 중심 기준 일정한 간격으로 표창  1, 3개 소환
	for (int i = -InStarCount; i <= InStarCount; i++)
	{
		// #2. 등록한 표창 클래스로부터 SpawnActor
		ACActor_Sub_Star* star = CHelpers::MySpawnActor<ACActor_Sub_Star>(starClass, GetOwner(), GetOwner()->GetTransform());
		//ACActor_Sub_Star* star = GetWorld()->SpawnActor<ACActor_Sub_Star>(starClass, GetOwner()->GetTransform(), SpawnParameters);

		// #3. Owner의 정면위치에서 일정한 간격을 두고 소환할때 필요한 값들
		const FVector& ownerLocation = OwnerCharacter->GetActorLocation();
		const FRotator& rotator = OwnerCharacter->GetActorRotation();
		FVector rightVector = FQuat(rotator).GetRightVector();

		// #4. 일정한 간격으로 나누고 enum 나열자 둔갑술(enum hack)을 이용하여 위치 결정하기
		float interval = static_cast<float>(INTERVAL) * i;
		rightVector *= interval;
		FVector ResultLocation = ownerLocation + rightVector;

		star->SetActorLocation(ResultLocation);
	}
}


