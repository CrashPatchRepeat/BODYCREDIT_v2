﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBullet.generated.h"

UCLASS()
class BODYCREDIT_V2_API ACBullet : public AActor
{
	GENERATED_BODY()

public:
	ACBullet();

	virtual void Tick(float DeltaTime) override;

	void Shoot(const FVector& InDirection);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileComponent;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = "Effect")
	class UNiagaraSystem* BloodEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	class UMaterialInterface* BulletDecalMaterial;

	UPROPERTY(EditAnywhere, Category = "Effect")
	float DecalSize = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Effect")
	float DecalLifeTime = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	UMaterialInstanceConstant* HitDecal;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	float Damage = 20.0f;
};
