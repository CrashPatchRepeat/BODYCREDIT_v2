#include "Components/Inventory/CInventoryBaseComponent.h"

#include "Components/Inventory/CInventoryTile.h"
#include "Items/Market/Base/CItemBase.h"
#include "Items/Market/Base/CItemObject.h"

bool UCInventoryBaseComponent::TryAddItem(UCItemObject* ItemObject)
{
	if (IsValid(ItemObject))
	{
		for (int32 i = 0; i < Items.Num(); ++i)
		{
			if (IsRoomAvailable(ItemObject, i))
			{
				AddItemAt(ItemObject, i);
				return true;
			}
		}
		ItemObject->Rotate();

		for (int32 i = 0; i < Items.Num(); ++i)
		{
			if (IsRoomAvailable(ItemObject, i))
			{
				AddItemAt(ItemObject, i);
				return true;
			}
		}

	}
	return false;
}

bool UCInventoryBaseComponent::IsRoomAvailable(UCItemObject* ItemObject, int32 TopLeftIndex)
{
	FInventoryTile ResultTile;

	FInventoryTile TempTile = IndexToTile(TopLeftIndex);
	FIntPoint TempDimension = ItemObject->GetDimension();

	for (int32 i = TempTile.X; i < (TempDimension.X + TempTile.X); ++i)
	{
		for (int32 j = TempTile.Y; j < (TempDimension.Y + TempTile.Y); ++j)
		{
			ResultTile.X = i;
			ResultTile.Y = j;

			if (!IsTileValid(ResultTile))
			{
				return false;
			}

			UCItemObject* TempItemObject = GetItemAtIndex(TileToIndex(ResultTile));
			if (IsValid(TempItemObject))
			{
				return false;
			}
		}
	}

	return true;
}

FInventoryTile UCInventoryBaseComponent::IndexToTile(const int32 Index)
{
	FInventoryTile TempTile;

	TempTile.X = Index % Columns;
	TempTile.Y = Index / Columns;

	return TempTile;
}

bool UCInventoryBaseComponent::IsTileValid(FInventoryTile& Tile)
{
	if (Tile.X >= 0 && Tile.Y >= 0 && Tile.X < Columns && Tile.Y < Rows)
		return true;

	return false;
}

UCItemObject* UCInventoryBaseComponent::GetItemAtIndex(int32 Index)
{
	if (Items.IsValidIndex(Index))
		return Items[Index];

	return nullptr;
}

int32 UCInventoryBaseComponent::TileToIndex(FInventoryTile& Tile)
{
	return Tile.X + (Tile.Y * Columns);
}

void UCInventoryBaseComponent::AddItemAt(UCItemObject* ItemObject, int32 TopLeftIndex)
{
	ItemObject->OwnerInventoryComp = this;

	FInventoryTile ResultTile;

	FInventoryTile TempTile = IndexToTile(TopLeftIndex);
	FIntPoint TempDimension = ItemObject->GetDimension();

	ItemObject->ItemData.StartPosition = FIntPoint(TempTile.X, TempTile.Y);

	for (int32 i = TempTile.X; i < (TempDimension.X + TempTile.X); ++i)
	{
		for (int32 j = TempTile.Y; j < (TempDimension.Y + TempTile.Y); ++j)
		{
			ResultTile.X = i;
			ResultTile.Y = j;
			if (IsTileValid(ResultTile))
			{
				int32 CurrentIndex = TileToIndex(ResultTile);
				Items[CurrentIndex] = ItemObject;
				ItemObject->CurrentIndex = CurrentIndex;
				IndexToObject.Add(CurrentIndex, ItemObject);
			}
		}
	}
	
	IsDirty = true;
}

void UCInventoryBaseComponent::ResetInventoryItem()
{
	for (auto& Item : Items)
	{
		if (Item)
		{
			if (Item->ItemActorOwner.IsValid())
			{
				Item->ItemActorOwner->Destroy();
			}
			Item->ItemActorOwner = nullptr;
		}
		Item = nullptr;
	}
}

TMap<UCItemObject*, FInventoryTile> UCInventoryBaseComponent::GetAllItems()
{
	TMap<UCItemObject*, FInventoryTile> AllItem;
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (IsValid(Items[i]))
		{
			if (!AllItem.Contains(Items[i]))
			{
				AllItem.Add(Items[i], IndexToTile(i));
			}
		}
	}
	return AllItem;
}

void UCInventoryBaseComponent::RemoveItem(UCItemObject* ItemObject)
{
	if (IsValid(ItemObject))
	{
		for (int i = 0; i < Items.Num(); ++i)
		{
			if (ItemObject == Items[i])
			{
				Items[i] = nullptr;
				IsDirty = true;
			}
		}
	}
}
