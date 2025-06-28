#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CInventoryItemWidget.generated.h"

class UCItemObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, UCItemObject*, RemovedItem);

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API UCInventoryItemWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	FOnItemRemoved OnItemRemoved;

	float TileSize = 75.f;
	UPROPERTY()
	TObjectPtr<UCItemObject> ItemObject;
};
