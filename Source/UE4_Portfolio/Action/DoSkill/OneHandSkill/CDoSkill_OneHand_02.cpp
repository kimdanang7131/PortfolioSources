#include "Action/DoSkill/OneHandSkill/CDoSkill_OneHand_02.h"
#include "Global.h"
////////////////////////
#include "Components/CPostProcessComponent.h"
////////////////////////
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
////////////////////////

ACDoSkill_OneHand_02::ACDoSkill_OneHand_02()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	Box   = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(Scene);

	Decal = CreateDefaultSubobject<UDecalComponent>("Decal");
	Decal->SetupAttachment(Scene);

	index = 1;
	coolTime = 11.f;
}

/** ��ų ���� �߿� �����ϼ� �ֵ��� ���� */
void ACDoSkill_OneHand_02::BeginPlay()
{
	Super::BeginPlay();
	bCanRotate = true;

	OnActorBeginOverlap.AddDynamic(this, &ACDoSkill_OneHand_02::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACDoSkill_OneHand_02::ActorEndOverlap);
}

void ACDoSkill_OneHand_02::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


/** Ȧ�� �� ȸ���ϴ� ���� Skill Range�� �Բ� ȸ���ϸ� Box�� ���� ���� 
    PostProcess�� ���̶���Ʈ ��Ű�鼭 ���� �ȿ� �ִ��� �˷��� */
void ACDoSkill_OneHand_02::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(OwnerCharacter, OtherActor));

	// #1. �߰� �� OtherActor�� ����Ʈ���μ��� ������Ʈ�� �ִٸ� ���̶���Ʈ
	UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(OtherActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
	if (!!PostProcess)
		PostProcess->DrawRenderingRed();

	OverlappedActors.Add(OtherActor);
}

/** Ȧ�� �� ȸ���ϴ� ���� Skill Range�� �Բ� ȸ���ϸ� Box���� ������ ���� 
    PostProcess�� ���̶���Ʈ �����ϸ鼭 ���� ������ �������� �˷��� */
void ACDoSkill_OneHand_02::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TRUE_RETURN(CSub::CheckActorBeginOverlapped(OwnerCharacter, OtherActor));

	// #1. ���� ��� Actor�� �ִٸ� ������ index ����
	int32 Index = OverlappedActors.Find(OtherActor);

	// #2. ������ Index�� �迭 �ȿ� �ִٸ� ����Ʈ���μ��� ������Ʈ�� �ִ��� Ȯ���ؼ� ���̶���Ʈ ���������ֱ�
	if (Index != INDEX_NONE)
	{
		UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(OtherActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
		if (!!PostProcess)
			PostProcess->ResetOutline();

		OverlappedActors.RemoveAt(Index);
	}
}

/** ��ų Ȱ��ȭ �Ǵ� ���� Skill Range�� �ٴڿ� ��� ( Ȧ���ϴ� ���� ) */
void ACDoSkill_OneHand_02::Activate()
{
	Super::Activate();

	// #1. Skill Range ǥ�� , ���� �ȿ� �ִ� Actor�� Box�� Ž��
	Decal->SetVisibility(true);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
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
void ACDoSkill_OneHand_02::Deactivate()
{
	Super::Deactivate();
	OverlappedActors.Empty();
}

/** Ȧ���� ������ ȸ���� ���߰� Skill Range�� �ִ� Actor�� �� ���� �� �Ÿ��� ���ͱ����� �̵� */
void ACDoSkill_OneHand_02::End_Hold()
{
	Super::End_Hold();

	// #1. ���ܻ�Ȳ) Nocollison���� �ٲٴ� ���� ���͵��� ��� EndOverlap�� �Ǿ� OverlappedActors�� �ʱ�ȭ �Ǵ� 
	//              ���ܰ� �־ �̸� ���������� OverlappedActors�� �����ϰ� Nocollision, Skill Range ����
	TArray<AActor*> SaveActors = OverlappedActors;
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Decal->SetVisibility(false);

	// #2. Ȧ�� �����ϴ� ���� ������ ���Ͱ� ���ٸ� return
	if (!SaveActors.Num())
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection("EndMontage", Data.AnimMontage);
		return;
	}

	// #3. ������ ���͵� �� ���� �Ÿ��� �� ���� ��������
	AActor* FarthestActor = CSub::FindFarthestActor(OwnerCharacter, SaveActors);

	// #4. ���� �Ÿ��� �� ���� �ڷ� �̵��ϰ��� ���� �� ���� ���� ���� �Ÿ� ���ϱ�
	FVector distance = FarthestActor->GetActorLocation() - OwnerCharacter->GetActorLocation();
	FVector forward  = CSub::GetCustomForwardVector(OwnerCharacter);
	forward *= 300.f;
	FVector TargetRelativeLocation = distance + OwnerCharacter->GetActorLocation() + forward;


	// #5. ���� �Ÿ����� MoveComponentTo �ϱ� -> 0.22f
	FLatentActionInfo LatentInfo; // Latent == �����ִ�
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(OwnerCharacter->GetRootComponent(), TargetRelativeLocation, FRotator(0, 0, 0), false, true, 0.22f, false, EMoveComponentAction::Type::Move , LatentInfo);

	// #6. Skill Range�� �ִ� Actor�� ��� ������ ���ϱ�
	for (AActor* DamageActor : SaveActors)
	{
		if(CSub::CustomTakeDamage(Data.Power , DamageActor, OwnerCharacter->GetController(), this))
		{
			if (!!HitEffect)
			{
				FTransform transfrom = FTransform(FRotator::ZeroRotator, DamageActor->GetActorLocation(), FVector(1.f));
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, transfrom, false, EPSCPoolMethod::AutoRelease);
			}

			if (!!HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, HitSound, DamageActor->GetActorLocation());
			}
		}

		// #7. ����Ʈ ���μ��� ������Ʈ �ִٸ� ���̶���Ʈ ��� ����
		UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(DamageActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
		PostProcess->ResetOutline();
	}
}

