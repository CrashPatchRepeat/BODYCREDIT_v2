#pragma once

#include "CoreMinimal.h"
#include "Components/CBaseComponent.h"
#include "CInventoryBaseComponent.generated.h"

class UCItemObject;
struct FInventoryTile;

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API UCInventoryBaseComponent : public UCBaseComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	int32 Columns = 4.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	int32 Rows = 5.f;
	
	UPROPERTY()
	TMap<int32, UCItemObject*> IndexToObject;
	
	bool TryAddItem(UCItemObject* ItemObject);
	void ResetInventoryItem();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<UCItemObject*> Items;
	
	bool IsDirty = false;

	bool IsRoomAvailable(UCItemObject* ItemObject, int32 TopLeftIndex);
	FInventoryTile IndexToTile(int32 Index);
	bool IsTileValid(FInventoryTile& Tile);
	UCItemObject* GetItemAtIndex(int32 Index);
	int32 TileToIndex(FInventoryTile& Tile);

	void AddItemAt(UCItemObject* ItemObject, int32 TopLeftIndex);
};
