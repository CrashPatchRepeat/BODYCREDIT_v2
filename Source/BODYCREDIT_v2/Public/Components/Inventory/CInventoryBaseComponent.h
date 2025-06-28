#pragma once

#include "CoreMinimal.h"
#include "Components/CBaseComponent.h"
#include "CInventoryBaseComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

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
	FOnInventoryChanged InventoryChanged;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	int32 Columns = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	int32 Rows = 15.f;
	
	UPROPERTY()
	TMap<int32, UCItemObject*> IndexToObject;
	
	bool TryAddItem(UCItemObject* ItemObject);
	void ResetInventoryItem();

	TMap<UCItemObject*, FInventoryTile> GetAllItems();

	int32 TileToIndex(FInventoryTile& Tile);

	bool IsRoomAvailable(UCItemObject* ItemObject, int32 TopLeftIndex);
	
	void RemoveItem(UCItemObject* ItemObject);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<UCItemObject*> Items;
	
	bool IsDirty = false;
	
	FInventoryTile IndexToTile(int32 Index);
	bool IsTileValid(FInventoryTile& Tile);
	UCItemObject* GetItemAtIndex(int32 Index);	

	void AddItemAt(UCItemObject* ItemObject, int32 TopLeftIndex);
};
