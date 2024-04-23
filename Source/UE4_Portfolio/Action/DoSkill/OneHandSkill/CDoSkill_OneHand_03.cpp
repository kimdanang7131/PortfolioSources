#include "Action/DoSkill/OneHandSkill/CDoSkill_OneHand_03.h"
#include "Global.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"

#include "Components/CPostProcessComponent.h"

ACDoSkill_OneHand_03::ACDoSkill_OneHand_03()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(Scene);

	Decal = CreateDefaultSubobject<UDecalComponent>("Decal");
	Decal->SetupAttachment(Scene);

	index = 2;
}

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

void ACDoSkill_OneHand_03::Activate()
{
	Super::Activate();

	bCanRotate = true;
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	Decal->SetVisibility(true);

	//Box->Location
	FVector start = Box->GetComponentLocation();
	FVector end;

	FVector up = CSub::GetCustomUpVector(this);
	up *= -90.f;

	end = start + up;

	TArray<TEnumAsByte<EObjectTypeQuery>> queries;
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	queries.Add(WorldStatic);

	TArray<AActor*> ignoreActors;

	FHitResult HitResult;
	if (UKismetSystemLibrary::LineTraceSingleForObjects(OwnerCharacter->GetWorld(), start, end, queries, false, ignoreActors, EDrawDebugTrace::None, HitResult, true))
	{
		Decal->SetWorldLocation(HitResult.ImpactPoint);
	}
}

void ACDoSkill_OneHand_03::Deactivate()
{
	Super::Deactivate();

	OverlappedActors.Empty();
}

void ACDoSkill_OneHand_03::End_Hold()
{
	bCanRotate = false;
	AActor* TargetTemp = TargetActor;
	Decal->SetVisibility(false);

	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (TargetTemp == nullptr)
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection("EndMontage", Data.AnimMontage);
		return;
	}

	FVector TargetLocation = TargetTemp->GetActorLocation();
	FVector TargetForward = CSub::GetCustomForwardVector(TargetTemp);
	FRotator NewRotation = TargetTemp->GetActorRotation();

	TargetForward *= -100.f;

	FVector NewLocation = TargetLocation + TargetForward;
	OwnerCharacter->SetActorLocationAndRotation(NewLocation, NewRotation);

	Super::End_Hold();


	UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(TargetTemp->GetComponentByClass(UCPostprocessComponent::StaticClass()));
	PostProcess->ResetOutline();

	TargetTemp = nullptr;
}


void ACDoSkill_OneHand_03::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	CSub::CheckActorBeginOverlapped(OwnerCharacter, OtherActor);

	OverlappedActors.Add(OtherActor);
	//CLog::Print(OtherActor->GetName());
	int32 Index = OverlappedActors.Find(OtherActor);

	
	AActor* tempActor  = CSub::FindNearestActor(OwnerCharacter, OverlappedActors);

	// 가장 가까운 액터 다시 검출했을때 이미 있는 TargetAcotr 중복도 아니고 겹친게 있을때
	if (tempActor != TargetActor && OverlappedActors.Num() > 0)
	{
		if (TargetActor != nullptr)
		{
			UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(TargetActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
			PostProcess->ResetOutline();
		}

	}

	TargetActor = tempActor;

	UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(TargetActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
	PostProcess->DrawRenderingRed();
}

void ACDoSkill_OneHand_03::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	CSub::CheckActorBeginOverlapped(OwnerCharacter, OtherActor);
	int32 Index = OverlappedActors.Find(OtherActor);

	if (Index != INDEX_NONE)
	{
		UCPostprocessComponent* PostProcess = Cast<UCPostprocessComponent>(OtherActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
		PostProcess->ResetOutline();

		if (OtherActor == TargetActor)
			TargetActor = nullptr;

		OverlappedActors.RemoveAt(Index);

		// 지우고나서도 OverlappedActors안에 요소가 있을 때
		if (OverlappedActors.Num() > 0)
		{
			AActor* tempActor = CSub::FindNearestActor(OwnerCharacter, OverlappedActors);
			
			if (!!tempActor)
			{
				if (tempActor != TargetActor && TargetActor != nullptr)
				{
					UCPostprocessComponent* PostProcess2 = Cast<UCPostprocessComponent>(TargetActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
					PostProcess2->ResetOutline();
				}

				TargetActor = tempActor;

				UCPostprocessComponent* PostProcess3 = Cast<UCPostprocessComponent>(TargetActor->GetComponentByClass(UCPostprocessComponent::StaticClass()));
				PostProcess3->DrawRenderingRed();
			}
		}
	}
}