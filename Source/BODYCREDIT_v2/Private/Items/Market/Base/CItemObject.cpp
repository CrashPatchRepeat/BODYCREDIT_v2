#include "Items/Market/Base/CItemObject.h"

#include "Components/Inventory/CLootInventoryComponent.h"
#include "Items/Market/Base/CItemBase.h"
#include "Items/Market/Base/CItemStrategy.h"

FIntPoint UCItemObject::GetDimension()
{
	FIntPoint Reverse;
	if (ItemData.Rotated) {
		Reverse.X = ItemData.Dimensions.Y;
		Reverse.Y = ItemData.Dimensions.X;
		return Reverse;
	}
	return ItemData.Dimensions;
}

UMaterialInterface* UCItemObject::GetIcon()
{
	if (ItemData.Rotated)
	{
		return ItemData.RotatedIcon;
	}
	return ItemData.Icon;
}

UMaterialInterface* UCItemObject::GetThumbnail()
{
	return ItemData.EquipedThumbnail;
}

TSubclassOf<ACItemBase> UCItemObject::GetItemClass()
{
	return ItemData.ItemClass;
}

void UCItemObject::Rotate()
{
	ItemData.Rotated = !ItemData.Rotated;
}

bool UCItemObject::IsRotated()
{
	return ItemData.Rotated;
}

FIntPoint UCItemObject::GetStartPosition()
{
	return ItemData.StartPosition;
}

FItemSaveData UCItemObject::ExportData() const
{
	FItemSaveData Data;
	Data.ID = ItemData.ID;
	Data.Dimensions = ItemData.Dimensions;
	Data.bRotated = ItemData.Rotated;
	Data.StartPosition = ItemData.StartPosition;
	Data.ItemClass = ItemData.ItemClass;
	Data.Icon = ItemData.Icon;
	Data.RotatedIcon = ItemData.RotatedIcon;
	Data.ItemType = ItemData.ItemType;
	Data.CurrentIndex = CurrentIndex;

	if (ItemActorOwner.IsValid() && ItemActorOwner->GetLootInventoryComp())
	{
		TSet<UCItemObject*> UniqueItems;
		for (auto& Pair : ItemActorOwner->GetLootInventoryComp()->IndexToObject)
		{
			if (IsValid(Pair.Value))
			{
				FInventoryItemData SubItemData;
				SubItemData.ID = Pair.Value->ItemData.ID;
				SubItemData.Dimensions = Pair.Value->ItemData.Dimensions;
				SubItemData.bRotated = Pair.Value->ItemData.Rotated;
				SubItemData.StartPosition = Pair.Value->ItemData.StartPosition;
				SubItemData.ItemClass = Pair.Value->ItemData.ItemClass;
				SubItemData.Icon = Pair.Value->ItemData.Icon;
				SubItemData.RotatedIcon = Pair.Value->ItemData.RotatedIcon;
				SubItemData.ItemType = Pair.Value->ItemData.ItemType;
				SubItemData.CurrentIndex = Pair.Value->CurrentIndex;

				Data.ContainedItems.Add(SubItemData);
			}
		}
	}


	return Data;
}

void UCItemObject::ImportData(const FItemSaveData& Data)
{
	ItemData.ID = Data.ID;
	ItemData.Dimensions = Data.Dimensions;
	ItemData.Rotated = Data.bRotated;
	ItemData.StartPosition = Data.StartPosition;
	ItemData.ItemClass = Data.ItemClass;
	ItemData.Icon = Data.Icon;
	ItemData.RotatedIcon = Data.RotatedIcon;
	ItemData.ItemType = Data.ItemType;
	CurrentIndex = Data.CurrentIndex;
	ContainedItems = Data.ContainedItems;
}

void UCItemObject::SetItemStrategy(UCItemStrategy* NewStrategy)
{
	if (ItemStrategy != NewStrategy) ItemStrategy = NewStrategy;
}

void UCItemObject::UseItem()
{
	if (ItemStrategy) ItemStrategy->Use(this);
}

void UCItemObject::SetIconImages()
{
	switch (ItemData.Rarity)
	{
	case EItemRarity::Blank:
		ItemData.Icon = ItemData.IconStruct.BlankIcon;
		ItemData.RotatedIcon = ItemData.IconStruct.BlankRotatedIcon;
		ItemData.EquipedThumbnail = ItemData.IconStruct.BlankThumbnail;
		break;
	case EItemRarity::Common:
		ItemData.Icon = ItemData.IconStruct.CommonIcon;
		ItemData.RotatedIcon = ItemData.IconStruct.CommonRotatedIcon;
		ItemData.EquipedThumbnail = ItemData.IconStruct.CommonThumbnail;
		break;
	case EItemRarity::Rare:
		ItemData.Icon = ItemData.IconStruct.RareIcon;
		ItemData.RotatedIcon = ItemData.IconStruct.RareRotatedIcon;
		ItemData.EquipedThumbnail = ItemData.IconStruct.RareThumbnail;
		break;
	case EItemRarity::Epic:
		ItemData.Icon = ItemData.IconStruct.EpicIcon;
		ItemData.RotatedIcon = ItemData.IconStruct.EpicRotatedIcon;
		ItemData.EquipedThumbnail = ItemData.IconStruct.EpicThumbnail;
		break;
	case EItemRarity::Legendary:
		ItemData.Icon = ItemData.IconStruct.LegendaryIcon;
		ItemData.RotatedIcon = ItemData.IconStruct.LegendaryRotatedIcon;
		ItemData.EquipedThumbnail = ItemData.IconStruct.LegendaryThumbnail;
		break;
	}
}

int32 UCItemObject::GetSellPrice()
{
	if (ItemData.ItemType == EPlayerPart::Basic)
	{
		return ItemData.Price;
	}

	float Multiplier = 0.0f;

	switch (ItemData.Rarity)
	{
	case EItemRarity::Common:
		Multiplier = 0.3f;
		break;
	case EItemRarity::Rare:
		Multiplier = 0.5f;
		break;
	case EItemRarity::Epic:
		Multiplier = 0.7f;
		break;
	case EItemRarity::Legendary:
		Multiplier = 0.9f;
		break;
	default:
		Multiplier = 0.1f;
		break;
	}

	return FMath::RoundToInt(ItemData.Price * Multiplier);
}

void UCItemObject::SetItemStat()
{
	float RarityMultiplier = 1.0f;

	switch (ItemData.Rarity)
	{
	case EItemRarity::Common:
		RarityMultiplier = 1.0f;
		break;
	case EItemRarity::Rare:
		RarityMultiplier = 1.5f;
		break;
	case EItemRarity::Epic:
		RarityMultiplier = 2.0f;
		break;
	case EItemRarity::Legendary:
		RarityMultiplier = 3.0f;
		break;
	default:
		RarityMultiplier = 1.0f;
		break;
	}

	FItemStatIncrease& Stat = ItemData.StatIncrease;

	if (Stat.Health > 0.f)      Stat.Health *= RarityMultiplier;
	if (Stat.Strength > 0.f)    Stat.Strength *= RarityMultiplier;
	if (Stat.Stamina > 0.f)     Stat.Stamina *= RarityMultiplier;
	if (Stat.Armor > 0.f)       Stat.Armor *= RarityMultiplier;
	if (Stat.CarryWeight > 0.f) Stat.CarryWeight *= RarityMultiplier;
}
