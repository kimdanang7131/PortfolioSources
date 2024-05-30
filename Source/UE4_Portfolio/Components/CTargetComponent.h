#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CTargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PORTFOLIO_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCTargetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
