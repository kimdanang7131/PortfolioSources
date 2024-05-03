#include "CMatineeCameraShake.h"
#include "Global.h"

UCMatineeCameraShake::UCMatineeCameraShake()
{
	OscillationDuration    = 0.25f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.1f;

	LocOscillation.Y.Amplitude = 40.f;
	LocOscillation.Y.Frequency = 4.5f;

	LocOscillation.Z.Amplitude = 40.f;
	LocOscillation.Z.Frequency = 4.5f;
}

void UCMatineeCameraShake::SetTiming(float duration, float blendIn, float blendOut)
{
	OscillationDuration     = duration;
	OscillationBlendInTime  = blendIn;
	OscillationBlendOutTime = blendOut;
}

void UCMatineeCameraShake::SetLocOsillationY(float ampliitude , float frequency )
{
	LocOscillation.Y.Amplitude = ampliitude;
	LocOscillation.Y.Frequency = frequency;
}

void UCMatineeCameraShake::SetLocOsillationZ(float ampliitude, float frequency)
{
	LocOscillation.Z.Amplitude = ampliitude;
	LocOscillation.Z.Frequency = frequency;
}
