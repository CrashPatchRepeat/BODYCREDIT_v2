#include "Components/Market/CMarketComponent.h"

#include "Games/CGameInstance.h"
#include "Items/Market/Base/CMarketStruct.h"

void UCMarketComponent::BeginPlay()
{
	Super::BeginPlay();
	GI = GetWorld()->GetGameInstance<UCGameInstance>();
}

TArray<FItemData> UCMarketComponent::GetMarketItems(EPlayerPart ItemType)
{
	TArray<FItemData> Weapons;
	
	if (!GI->GetItemDataTable())
		return Weapons;

	TArray<FItemData*> AllItems;
	GI->GetItemDataTable()->GetAllRows<FItemData>(TEXT("Weapon Load"), AllItems);

	for (FItemData* Item : AllItems)
	{
		if (Item && Item->ItemType == ItemType)
		{
			Weapons.Add(*Item);
		}
	}
	return Weapons;
}


