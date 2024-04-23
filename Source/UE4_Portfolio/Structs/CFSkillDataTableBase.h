#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enums/CEWeaponStateType.h"
#include "CFSkillDataTableBase.generated.h"

USTRUCT(BlueprintType)
struct FSkillDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UTexture2D* SkillTexture = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FText SkillDesc = FText::GetEmpty();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool bUnlocked = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 SkillIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        EWeaponStateType WeaponType;
};