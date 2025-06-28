#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/CInventoryBaseComponent.h"
#include "CInventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API UCInventoryComponent : public UCInventoryBaseComponent
{
	GENERATED_BODY()

public:
	float GetInventoryTileSize() const {return InventoryTileSize;}
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float InventoryTileSize = 75.f;
};
