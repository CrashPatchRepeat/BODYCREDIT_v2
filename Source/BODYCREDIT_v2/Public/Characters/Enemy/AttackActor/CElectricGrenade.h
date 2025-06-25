#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CElectricGrenade.generated.h"

class UProjectileMovementComponent;
class UNiagaraComponent;
class ACNox_EBase;

UCLASS()
class BODYCREDIT_V2_API ACElectricGrenade : public AActor
{
	GENERATED_BODY()
	
public:
	void InitializeGrenade(const FVector& InStartLocation, const FVector& InTargetLocation, const FVector& InVelocity,
						   float InMaxSpeed = 1500);
	
private:
	ACElectricGrenade();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

#pragma region 컴포넌트
	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileComp;
	UPROPERTY(EditAnywhere, Category=FX)
	UNiagaraComponent* FlashFX;
#pragma endregion

	UPROPERTY()
	ACNox_EBase* OwnerAI;

#pragma region Use Grenade
	FVector TargetLocation;
	float TriggerRadius = 100.f;
	bool bExploded = false;
	
	void Init(bool bInit);
	void UseFX(bool bUse);
	void Explode();
#pragma endregion
};
