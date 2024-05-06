#include "Action/DoSkill/KatanaSkill/CDoSkill_Katana_02.h"
#include "Global.h"
/////////////////////////////
#include "Components/CStatusComponent.h"
/////////////////////////////
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"

ACDoSkill_Katana_02::ACDoSkill_Katana_02()
{ 
	PrimaryActorTick.bCanEverTick = true; 

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	Box   = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(Scene);

	Decal = CreateDefaultSubobject<UDecalComponent>("SkillRange");
	Decal->SetupAttachment(Scene);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>("NPS_Stab");
	Niagara->SetupAttachment(Scene);

	index = 1;
}

void ACDoSkill_Katana_02::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ACDoSkill_Katana_02::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACDoSkill_Katana_02::ActorEndOverlap);
}

void ACDoSkill_Katana_02::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/** ��ų Ȱ��ȭ �� �ٴڿ� SkillRange Deacl ǥ�� �� �� ������ ����Ʈ + ���� ���� ��Ÿ �˱� -> �˱�� Notify�� ���� */
void ACDoSkill_Katana_02::Activate()
{
	Super::Activate();

	Decal->SetVisibility(true);
	FVector upVector = CSub::GetCustomUpVector(this);
	upVector *= -90.f;

	// #1. �ڽ� �Ʒ��� Trace ��� ���� ������
	FVector start = Box->GetComponentLocation();
	FVector end   = start + upVector;

	// #2.ECollisionChannel�� �ٴڿ� ���� Enum�� ObjectType���� ��ȯ�Ͽ� Query�� �߰�����
	TArray<TEnumAsByte<EObjectTypeQuery>> queries;
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	queries.Add(WorldStatic);

	TArray<AActor*> ignoreActors;
	FHitResult HitResult;

	// #3. �ڽ� �Ʒ��� Trace�� ��� �ٴ�(WorldStatic)�̶�� SkillRange( Decal ) ǥ��
	if (UKismetSystemLibrary::LineTraceSingleForObjects(OwnerCharacter->GetWorld(), start, end, queries, false, ignoreActors, EDrawDebugTrace::None, HitResult, true))
	{
		Decal->SetWorldLocation(HitResult.ImpactPoint);
	}
}

void ACDoSkill_Katana_02::Deactivate()
{
	Super::Deactivate();
	bCanRotate = true;
}

/** Ȧ�带 ������ ���� ȸ�� ���ϰ� �ϰ� SkillRange �Ⱥ��̵��� ���� */
void ACDoSkill_Katana_02::End_Hold()
{
	Super::End_Hold();
	bCanRotate = false;
	Decal->SetVisibility(false);
}

/**  */
void ACDoSkill_Katana_02::DoSubSkill()
{
	// #1. Stab Effect �����ְ� ������ ���� �� �ֵ��� Collision ����
	Niagara->Activate(true);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// #2. ���� Ÿ�̸� ���ٷ� �����Ͽ� Effect ��� ������ ���� Collision ���������Ͽ� ������ �ֵ���
	FTimerHandle Delay;
	float DelayTime = 0.1f;
	GetWorld()->GetTimerManager().SetTimer(Delay, FTimerDelegate::CreateLambda([&]()
		{
			Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			// TimerHandle �ʱ�ȭ
			GetWorld()->GetTimerManager().ClearTimer(Delay);
		}), DelayTime, false);	// �ݺ��Ϸ��� false�� true�� ����
}

void ACDoSkill_Katana_02::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(OwnerCharacter, OtherActor));

	CSub::CustomTakeDamage(Data.Power, OtherActor, GetOwner()->GetInstigatorController(), this);
}

void ACDoSkill_Katana_02::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

}