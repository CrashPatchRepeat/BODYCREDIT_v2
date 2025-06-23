#pragma once

#include "CoreMinimal.h"
#include "Characters/CNox.h"
#include "CNox_EBase.generated.h"

class ACNox_EController;
class UCNox_EStatusComp;
class UCNox_FSMComp;
class UCNox_EAnimInstance;
enum class EEnemyType : uint8;
enum class EEnemyState : uint8;
enum class ECombatState : uint8;
enum class EEnemyMovementSpeed : uint8;
enum class ESkillCoolDown : uint8;

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API ACNox_EBase : public ACNox
{
	GENERATED_BODY()

public:
#pragma region Component
	UCNox_EStatusComp* GetStatusComp();
#pragma endregion

#pragma region Get Sensing Function
	float GetSightRadius() const { return SightRadius; }
	float GetLoseSightRadius() const { return LoseSightRadius; }
	float GetPeripheralVisionAngleDegrees() const { return PeripheralVisionAngleDegrees; }
	float GetRetentionTime() const { return RetentionTime; }
	float GetHearingRange() const { return HearingRange; }
#pragma endregion

#pragma region Hearing Movement
	bool bHearingMovement = false;
	FVector HearingLoc = FVector::ZeroVector;
#pragma endregion

#pragma region Enemy Type
	EEnemyType GetEnemyType() const { return EnemyType; }
#pragma endregion

#pragma region FSM Set State
	void SetEnemyState(EEnemyState NewState);
	void SetCombatState(ECombatState NewCombatState);
#pragma endregion

#pragma region Apply Damage
	void SetApplyDamage(AActor* DamagedPlayer, const float DamageAmout);
#pragma endregion

#pragma region Target
	virtual void SetTarget(ACNox* InTarget);
	ACNox* GetTarget() const { return Target; }
#pragma endregion

#pragma region Set Movement Speed
	void SetMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed);
#pragma endregion

#pragma region Montage
	// Common
	UPROPERTY(EditDefaultsOnly, Category=Montage)
	UAnimMontage* HitMontage;
	UPROPERTY(EditDefaultsOnly, Category=Montage)
	UAnimMontage* DieMontage;
	// Zero, Medic
	UPROPERTY(EditDefaultsOnly, Category=Montage)
	UAnimMontage* AttackMontage;
	// Medic
	UPROPERTY(EditDefaultsOnly, Category=Montage)
	UAnimMontage* GrenadeMontage;
	UPROPERTY(EditDefaultsOnly, Category=Montage)
	UAnimMontage* ShieldMontage;
	// Memory Collector
	UPROPERTY(EditDefaultsOnly, Category=Montage)
	UAnimMontage* Attack1Montage;
	UPROPERTY(EditDefaultsOnly, Category=Montage)
	UAnimMontage* Attack2Montage;
	UPROPERTY(EditDefaultsOnly, Category=Montage)
	UAnimMontage* Attack3Montage;
	UPROPERTY(EditDefaultsOnly, Category=Montage)
	UAnimMontage* Attack4Montage;
	UPROPERTY(EditDefaultsOnly, Category=Montage)
	UAnimMontage* BeamMontage;
	UPROPERTY(EditDefaultsOnly, Category=Montage)
	UAnimMontage* WavePulseMontage;
#pragma endregion

#pragma region Attacking
	void HandleAttack();
	bool IsAttacking();

	virtual void AttackCollision(bool bOn, bool IsRightHand = true)
	{
	}
#pragma endregion

#pragma region Hitting
	void HandleHit(const int32 sectionIdx = 1);
	bool IsHitting();
	void ResetVal() const;
#pragma endregion

#pragma region Die
	void HandleDie(const int32 sectionIdx = 1);
	void SetLastHitImpulse(const FVector& InImpulse) { LastHitImpulse = InImpulse; }

	void SetLastHitInfo(const FVector& InLocation, const FName& InBoneName = NAME_None)
	{
		LastHitLocation = InLocation;
		LastHitBoneName = InBoneName;
	}
#pragma endregion

#pragma region Heal (Medic)
	void HealHP();
#pragma endregion

#pragma region Check Player In Forward Degree
	bool IsPlayerInForwardDegree(const float InForwardRange, const float InDegree = 10.f);
#pragma endregion

#pragma region FSM Skill Cool Downs
	void UpdateSkillCoolDowns(ESkillCoolDown Skill, float DeltaTime);
	bool IsSkillReady(ESkillCoolDown Skill) const;
	void UsingSkill(ESkillCoolDown Skill);
#pragma endregion

#pragma region Rotate To Target
	bool RotateToTarget(const float DeltaTime, const FTransform& CurTrans, const FVector& TargetLoc,
	                    float InteropSpeed = 5.f);
	void SetRotateToTarget();
#pragma endregion

#pragma region Extract Call Function
	UFUNCTION(BlueprintCallable)
	void ExtractCallFunction(ACNox* InTarget);
	void DayStart() { bExtractSucceed = false; }
	UFUNCTION(BlueprintCallable)
	void ExtractSucceed() { bExtractSucceed = true; }
#pragma endregion

#pragma region Sound
	void PlayIdleSound();
	void PlaySenseSound();
	void PlayAttackSound();
	void PlayGrenadeSound();
	void PlayHealSound();
	void PlayBeamSound();
	void PlayWavePulseSound();
	void PlayHitSound();
	void PlayDieSound();
#pragma endregion

protected:
#pragma region Init
	ACNox_EBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY()
	ACNox_EController* EnemyController;

	void InitComp();
#pragma endregion

#pragma region Debug
	UPROPERTY(EditDefaultsOnly, Category=Debug)
	bool bDebug = true;
#pragma endregion

#pragma region Component
	UPROPERTY(VisibleDefaultsOnly)
	UCNox_EAnimInstance* EnemyAnim;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCNox_EStatusComp* StatusComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCNox_FSMComp* FSMComp;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// class UAC_LootingInventoryComponent* LootInventoryComp;
#pragma endregion

#pragma region Sensing
	UPROPERTY(EditDefaultsOnly, Category="Sensing")
	float SightRadius = 300.f;
	UPROPERTY(EditDefaultsOnly, Category="Sensing")
	float LoseSightRadius = 400.f;
	UPROPERTY(EditDefaultsOnly, Category="Sensing")
	float PeripheralVisionAngleDegrees = 45.f;
	UPROPERTY(EditDefaultsOnly, Category="Sensing")
	float RetentionTime = 2.f;

	UPROPERTY(EditDefaultsOnly, Category="Sensing")
	float HearingRange = 800.f;

	virtual void SetPerceptionInfo()
	{
	}
#pragma endregion

#pragma region EnemyType
	UPROPERTY(EditDefaultsOnly)
	EEnemyType EnemyType;
#pragma endregion

#pragma region Target
	UPROPERTY(EditDefaultsOnly)
	ACNox* Target = nullptr;
#pragma endregion

#pragma region Set Movement Speed
	virtual void GetNewMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed, float& OutNewSpeed,
	                                 float& OutNewAccelSpeed)
	{
	}
#pragma endregion

#pragma region Heal (Medic)
	UPROPERTY(EditDefaultsOnly, Category=Health)
	float HealAmount = 10.f;
#pragma endregion

#pragma region Sound
	UPROPERTY(VisibleAnywhere)
	class UAudioComponent* SoundComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* IdleSoundCue;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* SenseSoundCue;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* AttackSoundCue;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* GrenadeSoundCue;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* HealSoundCue;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* BeamSoundCue;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* WavePulseSoundCue;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* HitSoundCue;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* DieSoundCue;
#pragma endregion

private:
#pragma region Die
	FVector LastHitImpulse = FVector::ZeroVector;
	FVector LastHitLocation = FVector::ZeroVector;
	FName LastHitBoneName = NAME_None;

	void SetRagdoll();
#pragma endregion

#pragma region Extract Call Function
	bool bExtractSucceed = true;
#pragma endregion
};
