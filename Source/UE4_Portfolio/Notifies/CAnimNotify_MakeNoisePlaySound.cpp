#include "Notifies/CAnimNotify_MakeNoisePlaySound.h"
#include "Global.h"
#include "GameFramework/Actor.h"

#include "Sound/SoundCue.h"


FString UCAnimNotify_MakeNoisePlaySound::GetNotifyName_Implementation() const
{
	return "PlaySound";
}

void UCAnimNotify_MakeNoisePlaySound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	NULL_RETURN(MeshComp);
	NULL_RETURN(MeshComp->GetOwner());
	
	if (!!Sound)
	{
		MeshComp->GetOwner()->MakeNoise(Loudness);
		UGameplayStatics::PlaySoundAtLocation(this, Sound, MeshComp->GetOwner()->GetActorLocation());
	}
		//UGameplayStatics::SpawnSoundAtLocation(this, Sound, MeshComp->GetOwner()->GetActorLocation());
}
