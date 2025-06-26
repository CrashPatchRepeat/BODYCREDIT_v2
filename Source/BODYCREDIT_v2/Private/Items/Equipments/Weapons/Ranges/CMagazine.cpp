#include "Items/Equipments/Weapons/Ranges/CMagazine.h"
#include "Global.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"

ACMagazine::ACMagazine()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh_Full, "Mesh_Full", Root);
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh_Empty, "Mesh_Empty", Root);
}

void ACMagazine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACMagazine::SetEject()
{
	bEject = true;

	Mesh_Full->SetVisibility(false);
	Mesh_Empty->SetSimulatePhysics(true);
}

void ACMagazine::BeginPlay()
{
	Super::BeginPlay();

	if (bEject == false)
		Mesh_Full->SetVisibility(true);
}
