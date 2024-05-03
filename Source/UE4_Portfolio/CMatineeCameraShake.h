#pragma once

#include "CoreMinimal.h"
#include "MatineeCameraShake.h"
#include "CMatineeCameraShake.generated.h"

UCLASS()
class UE4_PORTFOLIO_API UCMatineeCameraShake : public UMatineeCameraShake
{
	GENERATED_BODY()
	
public:
	UCMatineeCameraShake();

public:
	void SetTiming(float duration = 0.25f, float blendIn = 0.1f, float blendOut = 0.1f);

	void SetLocOsillationY(float ampliitude = 40.f, float frequency = 4.5f);
	void SetLocOsillationZ(float ampliitude = 40.f, float frequency = 4.5f);
};
