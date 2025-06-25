#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "CNox_CCTV.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API ACNox_CCTV : public ACNox_EBase
{
	GENERATED_BODY()

public:
#pragma region Rotate CCTV
	void RotateCCTV(float DeltaTime);
#pragma endregion

#pragma region Die Effect
	UPROPERTY(EditDefaultsOnly, Category=FX)
	class UNiagaraSystem* DieEffect = nullptr;
	UPROPERTY(EditDefaultsOnly, Category=FX)
	class UNiagaraSystem* DieSpark = nullptr;
#pragma endregion

private:
	ACNox_CCTV();
	
#pragma region CCTV Mesh
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CCTVMesh;
#pragma endregion
	
#pragma region Rotate CCTV
	FRotator InitialRotation;

	UPROPERTY(EditAnywhere, Category = "CCTV")
	float MinYaw = -60.f;

	UPROPERTY(EditAnywhere, Category = "CCTV")
	float MaxYaw = 60.f;

	UPROPERTY(EditAnywhere, Category = "CCTV")
	float RotationSpeed = 30.f;

	UPROPERTY(EditAnywhere, Category = "CCTV")
	float PauseTimeAtEnds = 1.5f;

	bool bRotatingRight = true;
	float PauseTimer = 0.f;
	bool bIsPaused = false;

	float SumRotYaw = 0.f;
#pragma endregion

#pragma region Override Functions
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
							 class AController* EventInstigator, AActor* DamageCauser) override;
#pragma endregion

#pragma region Broadcast
	UFUNCTION()
	void BroadCastDetectPlayer(ACNox* DetectPlayer);
#pragma endregion
};
