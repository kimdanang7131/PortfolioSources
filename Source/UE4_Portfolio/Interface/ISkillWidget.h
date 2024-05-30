#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISkillWidget.generated.h"

UINTERFACE(MinimalAPI)
class UISkillWidget : public UInterface
{
	GENERATED_BODY()
};

class UE4_PORTFOLIO_API IISkillWidget
{
	GENERATED_BODY()

public:
	
	virtual void SetCoolPercent(const float& InPercent) = 0;
	virtual void SetProgressTranscluent(bool bIsTranscluent) = 0;
};
