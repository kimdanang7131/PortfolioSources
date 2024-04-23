#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle, Dodge, Action, Skill, Hitted, Dead, Max
};

