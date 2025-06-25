#include "Items/Equipments/Weapons/Ranges/CBullet.h"
#include "Global.h"
#include "Components/capsulecomponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACBullet::ACBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");
	Capsule->SetRelativeRotation(FRotator(90, 0, 0));
	Capsule->SetCapsuleHalfHeight(50);
	Capsule->SetCapsuleRadius(2);
	Capsule->SetCollisionProfileName("BlockAllDynamic");

	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh", Capsule);
	Mesh->SetRelativeScale3D(FVector(1, 0.025f, 0.025f));
	Mesh->SetRelativeRotation(FRotator(90, 0, 0));

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &ProjectileComponent, "ProjectileComponent");
	ProjectileComponent->InitialSpeed = 2e+4f;
	ProjectileComponent->MaxSpeed = 2e+4f;
	ProjectileComponent->ProjectileGravityScale = 0;
}

void ACBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACBullet::Shoot(const FVector& InDirection)
{
	SetLifeSpan(3);

	ProjectileComponent->Velocity = InDirection * ProjectileComponent->InitialSpeed;
	ProjectileComponent->SetActive(true);
}

void ACBullet::BeginPlay()
{
	Super::BeginPlay();

	ProjectileComponent->SetActive(false);
	Capsule->OnComponentHit.AddDynamic(this, &ACBullet::OnHit);
}

void ACBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}
