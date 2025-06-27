#include "Items/Market/Base/CItemBase.h"

#include "Items/Market/Base/CItemObject.h"
#include "Items/Market/Base/CMarketStruct.h"
#include "Global.h"
#include "Games/CGameInstance.h"
#include "Games/CMainGM.h"
#include "Items/Market/Base/CItemStrategy.h"

ACItemBase::ACItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UCapsuleComponent>(this, &CapsuleComp, "CapsuleComp", Root);
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMeshComp, "SkeletalMeshComp", CapsuleComp);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &StaticMeshComp, "StaticMeshComp", CapsuleComp);

	CapsuleComp->SetGenerateOverlapEvents(true);
	ItemObject = CreateDefaultSubobject<UCItemObject>(TEXT("ItemObject"));	
}

void ACItemBase::BeginPlay()
{
	Super::BeginPlay();

	ItemDataTable = GetGameInstance<UCGameInstance>()->GetItemDataTable();
	if (ItemDataTable)
	{
		FItemData* TempData = ItemDataTable->FindRow<FItemData>(ItemName, ContextString);
		if (ItemObject && TempData && TempData->Mesh)
		{
			ItemObject->ItemData = *TempData;
			StaticMeshComp->SetStaticMesh(ItemObject->ItemData.Mesh);
		}
	}

	if (!ItemObject)
		ItemObject = NewObject<UCItemObject>(this, TEXT("ItemObject"));
	
	ItemObject->ItemActorOwner = this;

	GameMode = Cast<ACMainGM>(GetWorld()->GetAuthGameMode());
}

void ACItemBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	ItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Item/DT_ItemData.DT_ItemData"));

	if (ItemDataTable)
	{
		FItemData* TempData = ItemDataTable->FindRow<FItemData>(ItemName, ContextString);
		if (ItemObject && TempData && TempData->Mesh)
		{
			ItemObject->ItemData = *TempData;
			StaticMeshComp->SetStaticMesh(ItemObject->ItemData.Mesh);
		}
	}
#endif
}

void ACItemBase::SetItemStrategy(UCItemStrategy* NewStrategy)
{
	if (ItemStrategy != NewStrategy) ItemStrategy = NewStrategy;
}

void ACItemBase::UseItem()
{
	if (ItemStrategy) ItemStrategy->Use(ItemObject);
}