#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "State/CMemoryData.h"
#include "CNox_Memory.generated.h"

class ACRangeProjectile;
class ACBeam;
class ACWavePulse;
class ACVent;
class ACStair;

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API ACNox_Memory : public ACNox_EBase
{
	GENERATED_BODY()

public:
#pragma region Beam
	void ShutBeam();
	bool IsPlayBeam();
	void BeamAttack();
	void BeamAttackEnd();
#pragma endregion

#pragma region Wave Pulse
	void ShutWavePulse();
	bool IsPlayWavePulse();
	void WavePulseAttack();
#pragma endregion

#pragma region Memory
	void RegisterMemory(const FMemoryFragment& InNewMemory);
	bool IsMemoryEmpty() const { return MemoryQueue.Num() > 0; }
	const FMemoryFragment& GetMemoryTarget() const { return CurrentTargetMemory; }
	bool EvaluateMemory();
	void SetMemoryTarget_MemoryMoveEnd(const FMemoryFragment& InNewMemory);
#pragma endregion

#pragma region Vent
	TArray<ACVent*> GetAllVent() const { return AllVent; }
#pragma endregion

#pragma region Stair
	TArray<ACStair*> GetAllStair() const { return AllStair; }
#pragma endregion

#pragma region Skill CoolDown
	UPROPERTY(EditDefaultsOnly, Category = "Skill CoolDown")
	float RangedCoolDown = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Skill CoolDown")
	float BeamCoolDown = 20.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Skill CoolDown")
	float WavePulseCoolDown = 20.0f;
#pragma endregion

#pragma region Projectile
	void StartRangeAttack(bool bIsRight);
	void ReturnToPool(ACRangeProjectile* ReturnedProjectile);
#pragma endregion
	
private:
	ACNox_Memory();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetPerceptionInfo() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetNewMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed, float& OutNewSpeed, float& OutNewAccelSpeed) override;
	
#pragma region Beam
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACBeam> BeamOrgCls;
	UPROPERTY(visibleAnywhere)
	ACBeam* Beam;
	bool bRotateToTarget = false;
#pragma endregion

#pragma region Wave Pulse
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACWavePulse> WavePulseOrgCls;
	UPROPERTY(visibleAnywhere)
	ACWavePulse* WavePulse;
#pragma endregion

#pragma region Memory
	UPROPERTY(EditAnywhere, Category="Memory")
	TArray<FMemoryFragment> MemoryQueue;
	UPROPERTY(EditAnywhere, Category="Memory")
	FMemoryFragment CurrentTargetMemory;	
	UPROPERTY(EditAnywhere, Category="Memory")
	float MemoryExpireTime = 15.0f;
#pragma endregion

#pragma region Vent
	UPROPERTY()
	TArray<ACVent*> AllVent;
#pragma endregion

#pragma region Stair
	UPROPERTY()
	TArray<ACStair*> AllStair;
#pragma endregion

#pragma region Projectile
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACRangeProjectile> RangeProjectileCls;
	UPROPERTY(VisibleAnywhere)
	TArray<ACRangeProjectile*> RangeProjectileArray;
	UPROPERTY(EditAnywhere)
	int32 SpawnProjectileCount = 16;
	UPROPERTY(EditAnywhere)
	FVector SpawnScale=FVector(1);
	
	void SpawnRangeProjectile();
#pragma endregion
};
