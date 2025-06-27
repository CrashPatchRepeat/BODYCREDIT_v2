#include "Components/Inventory/CLootInventoryComponent.h"

#include "Games/CGameInstance.h"
#include "Items/Market/Base/CItemBase.h"
#include "Items/Market/Base/CItemObject.h"
#include "Items/Market/Base/CMarketStruct.h"

void UCLootInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner() && GetOwner()->IsA(ACItemBase::StaticClass())) return;
	RefreshInventory();
}

void UCLootInventoryComponent::AddRandomItem()
{
	UCGameInstance* GI = GetWorld()->GetGameInstance<UCGameInstance>();
	if (!GI) return;

	if (GI->CachedItemLists.Common.Num() == 0 &&
		GI->CachedItemLists.Rare.Num() == 0 &&
		GI->CachedItemLists.Epic.Num() == 0 &&
		GI->CachedItemLists.Legendary.Num() == 0)
	{
		GI->InitItemCache(GI->GetItemDataTable());
	}

	const int32 NumToAdd = FMath::RandRange(1, 5);

	EItemRarity Rarity = GI->GetRandomRarityByLootTier(LootBoxTier);

	for (int i = 0; i < NumToAdd; ++i) 
	{
		const TArray<FItemData*>* Pool = nullptr;
		switch (Rarity)
		{
		case EItemRarity::Common:    Pool = &GI->CachedItemLists.Common;    break;
		case EItemRarity::Rare:      Pool = &GI->CachedItemLists.Rare;      break;
		case EItemRarity::Epic:      Pool = &GI->CachedItemLists.Epic;      break;
		case EItemRarity::Legendary: Pool = &GI->CachedItemLists.Legendary; break;
		default: break;
		}
		if (!Pool || Pool->Num() == 0) continue;

		FItemData* Chosen = (*Pool)[FMath::RandRange(0, Pool->Num() - 1)];
		if (!Chosen || !Chosen->ItemClass) continue;

		if (Chosen->ItemName.ToString().Contains(TEXT("AccessCard")))
		{
			if (GI->NumKeySpawned >= GI->MaxNumKey) continue;
		}

		ACItemBase* Spawned = GetWorld()->SpawnActor<ACItemBase>(Chosen->ItemClass);
		if (!IsValid(Spawned) || !IsValid(Spawned->ItemObject)) continue;

		if (TryAddItem(Spawned->ItemObject))
		{
			// Spawned->ItemObject->ItemData = *Chosen;
			Spawned->SetActorHiddenInGame(true);
			Spawned->SetActorEnableCollision(false);
		}
		else
		{
			Spawned->Destroy();
		}
	}
}

void UCLootInventoryComponent::RefreshInventory()
{
	ResetInventoryItem();
	AddRandomItem();
}
