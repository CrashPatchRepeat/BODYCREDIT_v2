#include "Games/CGameInstance.h"

#include "Items/Market/Base/CItemBase.h"
#include "Items/Market/Base/CMarketStruct.h"

void UCGameInstance::InitItemCache(UDataTable* ItemDT)
{
	if (!ItemDT) return;
	CachedItemLists = FItemListByRarity();

	TArray<FName> RowNames = ItemDT->GetRowNames();

	for (const FName& RowName : RowNames)
	{
		FItemData* Row = ItemDT->FindRow<FItemData>(RowName, TEXT("Cache"));
		if (!Row || !Row->ItemClass) continue;
		if (Row->ItemType != EPlayerPart::Basic) continue;

		switch (Row->Rarity)
		{
		case EItemRarity::Common: CachedItemLists.Common.Add(Row);
			break;
		case EItemRarity::Rare: CachedItemLists.Rare.Add(Row);
			break;
		case EItemRarity::Epic: CachedItemLists.Epic.Add(Row);
			break;
		case EItemRarity::Legendary: CachedItemLists.Legendary.Add(Row);
			break;
		default: break;
		}
	}
}

EItemRarity UCGameInstance::GetRandomRarityByLootTier(ELootBoxTier Tier)
{
	FRarityProbability* Prob = LootBoxRarityMap.Find(Tier);
	if (!Prob) return EItemRarity::Common;

	float Roll = FMath::FRand();
	float Accum = 0.f;

	if (Roll < (Accum += Prob->Common)) return EItemRarity::Common;
	if (Roll < (Accum += Prob->Rare)) return EItemRarity::Rare;
	if (Roll < (Accum += Prob->Epic)) return EItemRarity::Epic;
	if (Roll < (Accum += Prob->Legendary)) return EItemRarity::Legendary;

	return EItemRarity::Common;
}

void UCGameInstance::SetPlayerGold(int32 NewGold)
{
	PlayerGold = NewGold;
	OnGoldChanged.Broadcast(NewGold);
}
