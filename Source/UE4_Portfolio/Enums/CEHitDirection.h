#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Idle, Front, Back, Left, Right, Max
};