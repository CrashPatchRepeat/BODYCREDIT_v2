#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Items/Market/Base/CMarketStruct.h"
#include "CGameInstance.generated.h"

enum class ELootBoxTier : uint8;
struct FItemData;

/**
 * 
 */
USTRUCT()
struct FItemListByRarity
{
	GENERATED_BODY()

	TArray<FItemData*> Common;
	TArray<FItemData*> Rare;
	TArray<FItemData*> Epic;
	TArray<FItemData*> Legendary;

	FItemListByRarity()
	{
		Common.Empty();
		Rare.Empty();
		Epic.Empty();
		Legendary.Empty();
	}
};

USTRUCT(BlueprintType)
struct FRarityProbability
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Common = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rare = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Epic = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Legendary = 0.05f;
};

UCLASS()
class BODYCREDIT_V2_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	const int32 MaxNumKey = 1;	
	const int32 NumKeySpawned = 0;
	
	UPROPERTY()
	FItemListByRarity CachedItemLists;
	
	UDataTable* GetItemDataTable() const {return ItemDataTable;}
	void InitItemCache(UDataTable* ItemDT);
	EItemRarity GetRandomRarityByLootTier(ELootBoxTier Tier);
	
private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"), Category = "DataTable")
	UDataTable* ItemDataTable;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	TMap<ELootBoxTier, FRarityProbability> LootBoxRarityMap;
};
