#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CFItemDataTableBase.generated.h"

USTRUCT(BlueprintType)
struct FItemDataTableBase : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* ItemTexture = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText ItemName = FText::GetEmpty();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText ItemDescript = FText::GetEmpty();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bCanActivate = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bCanDrop = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Price = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Quantity = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<class ACItem> ItemClass;
};
