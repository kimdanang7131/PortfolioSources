#include "Action/DoSkill/OneHandSkill/CDoSkill_OneHand_03.h"
#include "Global.h"
/////////////////////////
#include "Components/CPostProcessComponent.h"
#include "Components/CStateComponent.h"
/////////////////////////
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"

ACDoSkill_OneHand_03::ACDoSkill_OneHand_03()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	Box   = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(Scene);

	Decal = CreateDefaultSubobject<UDecalComponent>("Decal");
	Decal->SetupAttachment(Scene);

	index = 2;
	coolTime = 19.f;
}

/** ��ų ���� �� ȸ�������ϵ��� ���� */
void ACDoSkill_OneHand_03::BeginPlay()
{
	Super::BeginPlay();
	bCanRotate = true;

	OnActorBeginOverlap.AddDynamic(this, &ACDoSkill_OneHand_03::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACDoSkill_OneHand_03::ActorEndOverlap);
}

void ACDoSkill_OneHand_03::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/** ������ Actor���� ���� ������ ���� ����� �Ÿ��� ���� ã�Ƽ� ����Ʈ ���μ��� ���̶���Ʈ ��Ű�� */
void ACDoSkill_OneHand_03::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(OwnerCharacter, OtherActor));

	OverlappedActors.Add(OtherActor);
	/*int32 index = OverlappedActors.Find(OtherActor);*/

	// #1. ���Ӱ� ������ Actor + ���� ������ Actors�� ���� ����� Actor ã��
	AActor* TempActor = CSub::FindNearestActor(OwnerCharacter, OverlappedActors);

	// #2. TargetActor( ���̶���Ʈ �� Actor ) �� ���Ӱ� ������ ���Ϳ� �ٸ��� -> TargetActor( ���̶���Ʈ �� Actor ) ���̶���Ʈ ���� ���� ��Ű��
	if (TempActor != TargetActor && OverlappedActors.Num() > 0)
	{
		if (TargetActor != nullptr)
		{
			UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(TargetActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
			if (!!PostProcess)
				PostProcess->ResetOutline();
		}
	}

	// #3. ������ ���̶���Ʈ ���� �۾� �������� TargetActor ���� �� ���̶���Ʈ
	TargetActor = TempActor;

	UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(TargetActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
	if (!!PostProcess)
		PostProcess->DrawRenderingRed();
}

/** ������ Actor���� ���� ������ ���̶���Ʈ ���� �� ���� ����� �Ÿ��� ���� ã�Ƽ� ����Ʈ ���μ��� ���̶���Ʈ ��Ű�� */
void ACDoSkill_OneHand_03::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(OwnerCharacter, OtherActor));

	// #1. ���� Actor Index ã��
	int32 fIndex = OverlappedActors.Find(OtherActor);

	// #2. OtherActor�� �迭 �ȿ� �ִٸ� ����
	if (fIndex != INDEX_NONE)
	{
		// #3. �ϴ� Target�̵� �ƴϵ� ����Ʈ ���μ��� ���̶���Ʈ ����
		UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(OtherActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
		if(!!PostProcess)
			PostProcess->ResetOutline();

		// #4. ���� Actor�� TargetActor ( ���̶���Ʈ �� Actor ) ���ٸ� TargetActor ����  
		if (OtherActor == TargetActor)
			TargetActor = nullptr;

		OverlappedActors.RemoveAt(fIndex);

		// #5. �����µ��� SkillRange �ȿ� ���͵��� �����ִٸ� ���� ����� ���� ã�Ƽ� ���̶���Ʈ
		if (OverlappedActors.Num() > 0)
		{
			AActor* TempActor = CSub::FindNearestActor(OwnerCharacter, OverlappedActors);

			if (!!TempActor)
			{
				TargetActor = TempActor;

				UCPostprocessComponent* TargetPostProcess = Cast<UCPostprocessComponent>(TargetActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
				if(!!TargetPostProcess)
					TargetPostProcess->DrawRenderingRed();
			}
		}
	}
}

/** ��ų Ȱ��ȭ �Ǵ� ���� Skill Range�� �ٴڿ� ��� ( Ȧ���ϴ� ���� ) */
void ACDoSkill_OneHand_03::Activate()
{
	Super::Activate();
	bCanRotate = true;

	// #1. Skill Range ǥ�� , ���� �ȿ� �ִ� Actor�� Box�� Ž��
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Decal->SetVisibility(true);

	// #2. Skill Range Decal�� ��� ���� ������ ����
	FVector up = CSub::GetCustomUpVector(this);
	up *= -90.f;
	FVector start = Box->GetComponentLocation();
	FVector end = start + up;

	TArray<TEnumAsByte<EObjectTypeQuery>> queries;
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	queries.Add(WorldStatic);

	TArray<AActor*> ignoreActors;

	// #3. Skill Range�� �ٴڿ� ���ð����ϸ� Decal ���� ( ���� ) -> �������� �Ϸ��� Tick���� �Űܾ���
	FHitResult HitResult;
	if (UKismetSystemLibrary::LineTraceSingleForObjects(OwnerCharacter->GetWorld(), start, end, queries, false, ignoreActors, EDrawDebugTrace::None, HitResult, true))
	{
		Decal->SetWorldLocation(HitResult.ImpactPoint);
	}
}

/** ��ų�� ��Ȱ��ȭ �Ǵ� ���� ���ܻ�Ȳ ó���� ���� OverlappedActors�� �ʱ�ȭ ��Ű�� */
void ACDoSkill_OneHand_03::Deactivate()
{
	Super::Deactivate();
	OverlappedActors.Empty();
}

/** Ȧ���� ������ ȸ���� ���߰� Skill Range�� �ִ� Actor�� �� ���� ����� �Ÿ��� ���ͷ� �̵� */
void ACDoSkill_OneHand_03::End_Hold()
{
	bCanRotate = false;

	// #1. ���ܻ�Ȳ) Nocollison���� �ٲٴ� ���� ���͵��� ��� EndOverlap�� �Ǿ� OverlappedActors�� �ʱ�ȭ �Ǵ� 
    //              ���ܰ� �־ �̸� ���������� OverlappedActors�� �����ϰ� Nocollision, Skill Range ����
	AActor* TargetTemp = TargetActor;
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Decal->SetVisibility(false);

	// #2. Ȧ�� �����ϴ� ���� Ÿ���� ���ٸ� return
	if (TargetTemp == nullptr)
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection("EndMontage", Data.AnimMontage);
		return;
	}

	// #3. Ÿ�� �ڷ� �̵���Ų �� Ÿ�� �Ĵٺ��� 
	FVector TargetLocation = TargetTemp->GetActorLocation();
	FVector TargetForward  = CSub::GetCustomForwardVector(TargetTemp);
	FRotator NewRotation   = TargetTemp->GetActorRotation();
	TargetForward *= -100.f;

	FVector NewLocation = TargetLocation + TargetForward;
	OwnerCharacter->SetActorLocationAndRotation(NewLocation, NewRotation);

	// #4. �̵��ϰ��� �ִϸ��̼� �����ؾ� �ϹǷ�
	Super::End_Hold();


	// #5. Ÿ���� ����Ʈ���μ��� ������Ʈ �ִٸ� ���̶���Ʈ ���������ֱ�
	UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(TargetTemp->GetComponentByClass(UCPostprocessComponent::StaticClass()));
	PostProcess->ResetOutline();

	TargetTemp  = nullptr;
	TargetActor = nullptr;
}


