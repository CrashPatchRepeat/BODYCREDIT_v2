#pragma once

#include "CoreMinimal.h"
#include "CInventoryTile.generated.h"

USTRUCT(BlueprintType)
struct FInventoryTile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;
};
