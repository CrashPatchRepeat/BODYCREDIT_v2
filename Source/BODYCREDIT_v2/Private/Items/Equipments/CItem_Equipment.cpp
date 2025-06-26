#include "Items/Equipments/CItem_Equipment.h"
#include "Global.h"
#include "Components/SkeletalMeshComponent.h"

ACItem_Equipment::ACItem_Equipment()
{
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, TEXT("Mesh"), RootComponent);
}

void ACItem_Equipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACItem_Equipment::BeginPlay()
{
	Super::BeginPlay();
}
