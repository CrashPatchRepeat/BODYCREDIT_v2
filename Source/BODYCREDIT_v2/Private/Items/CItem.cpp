#include "Items/CItem.h"
#include "Global.h"
#include "Components/SceneComponent.h"

ACItem::ACItem()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, TEXT("RootComponent"));
}

void ACItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACItem::BeginPlay()
{
	Super::BeginPlay();
}
