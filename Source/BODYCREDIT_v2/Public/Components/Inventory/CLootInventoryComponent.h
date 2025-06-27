#pragma once

#include "CoreMinimal.h"
#include "CInventoryBaseComponent.h"
#include "CLootInventoryComponent.generated.h"

UENUM(BlueprintType)
enum class ELootBoxTier : uint8
{
	Blank    UMETA(DisplayName = "Blank"),
	Common    UMETA(DisplayName = "Common"),
	Rare  UMETA(DisplayName = "Rare"),
	Epic      UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BODYCREDIT_V2_API UCLootInventoryComponent : public UCInventoryBaseComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ItemDataTable;

	void AddRandomItem();

	void RefreshInventory();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELootBoxTier LootBoxTier = ELootBoxTier::Common;
	
private:
	virtual void BeginPlay() override;
};
