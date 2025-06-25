#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRangeProjectile.generated.h"

class ACNox_Memory;
class UBoxComponent;
class UNiagaraComponent;

UCLASS()
class BODYCREDIT_V2_API ACRangeProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	void InitializeProjectile(FVector InStartLocation, class ACNox* InTargetActor);
	void SetCollisionEnabled(bool bEnabled);
	
private:	
	ACRangeProjectile();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	ACNox_Memory* OwnerAI;
	
#pragma region 컴포넌트
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxComp;
	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* ProjectileFxComp;
#pragma endregion

#pragma region Overlap
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
				   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
#pragma endregion

#pragma region Init
	FVector StartLocation;
#pragma endregion
	
#pragma region Attack
	UPROPERTY()
	ACNox* Target;
	FVector CurrentVelocity;
	float MaxTurnRateDegPerSec = 60.0f; // 너무 휘지 않도록 제한
	float Speed = 800.0f;
#pragma endregion
};
