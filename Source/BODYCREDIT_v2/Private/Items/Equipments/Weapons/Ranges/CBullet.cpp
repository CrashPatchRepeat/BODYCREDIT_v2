#include "Items/Equipments/Weapons/Ranges/CBullet.h"
#include "Global.h"
#include "Components/capsulecomponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Characters/CNox.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Components/DecalComponent.h"

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

	CHelpers::GetAsset<UNiagaraSystem>(&BloodEffect, TEXT("/Script/Niagara.NiagaraSystem'/Game/Items/Equipments/Weapons/Ranges/Effects/Niagara/Blood/NS_Blood_Splat_2D.NS_Blood_Splat_2D'"));

	CHelpers::GetAsset<UParticleSystem>(&HitParticle, "/Script/Engine.ParticleSystem'/Game/Items/Equipments/Weapons/Ranges/Effects/P_Impact_Default.P_Impact_Default'");

	CHelpers::GetAsset<UMaterialInstanceConstant>(&HitDecal, "/Script/Engine.MaterialInstanceConstant'/Game/Items/Equipments/Weapons/Ranges/Effects/Materials/MI_Hit.MI_Hit'");

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
	if (OtherActor and OtherActor != this and OtherComp)
	{
		// 🎯 캐릭터라면 데미지 전달 + 피 이펙트 + 사운드
		if (ACNox* HitCharacter = Cast<ACNox>(OtherActor))
		{
			if (BloodEffect)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					BloodEffect,
					Hit.ImpactPoint,
					Hit.ImpactNormal.Rotation(),
					FVector(0.3f)
				);
			}

			UGameplayStatics::ApplyPointDamage(
				HitCharacter,
				Damage,
				GetActorForwardVector(),
				Hit,
				GetInstigatorController(),
				this,
				UDamageType::StaticClass()
			);

			//// ✅ 사운드 재생 (Shooter 기준)
			//if (HitCharacterSound && GetInstigator())
			//{
			//	UGameplayStatics::PlaySound2D(GetInstigator(), HitCharacterSound);
			//}
		}
		//else
		//{
		//	if (HitDecal)
		//	{
		//		FRotator rotator = Hit.ImpactNormal.Rotation();
		// 		UDecalComponent* decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), HitDecal, FVector(5), Hit.Location, rotator, 10);
		//		decal->SetFadeScreenSize(0);
		//	}

		//	if (HitParticle)
		//	{
		//		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(Hit.Location, Hit.TraceStart);

		//		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Hit.Location, rotator);
		//	}
		//}
	}

	Destroy();
}
