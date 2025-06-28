#pragma once

#include "CoreMinimal.h"
#include "CInventoryLine.generated.h"

USTRUCT(BlueprintType)
struct FInventoryLineDir
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Start;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D End;
};
