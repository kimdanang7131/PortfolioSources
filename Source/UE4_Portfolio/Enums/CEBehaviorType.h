#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	Idle_BT, Dodge_BT, Action_BT, Skill_BT, Hitted_BT, Approach_BT, Patrol_BT , Max
};
