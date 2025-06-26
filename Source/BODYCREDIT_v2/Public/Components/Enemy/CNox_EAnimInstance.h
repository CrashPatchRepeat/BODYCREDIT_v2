#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CNox_EAnimInstance.generated.h"

class ACNox_EBase;

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API UCNox_EAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
#pragma region Grenade
	void PlayGrenadeMontage() const;
	bool IsPlayingGrenade() const;
#pragma endregion
	
#pragma region Attacking
	void PlayAttackMontage();
	bool IsAttacking() const;
#pragma endregion

#pragma region Heal
	void PlayHealMontage(const bool bInHealStart) const;
	bool IsHealing() const;
#pragma endregion

#pragma region Beam
	void PlayBeamAttack() const;
	void StopBeamAttack() const;
	bool IsBeamAttacking() const;
#pragma endregion

#pragma region Wave Pulse
	void PlayWavePulse() const;
	bool IsWavePulseAttacking() const;
#pragma endregion
	
#pragma region Hit
	void PlayHitMontage(const int32 sectionIdx = 1);
	bool IsHitting() const;
#pragma endregion
	
private:
#pragma region Override
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
#pragma endregion

#pragma region Common
	UPROPERTY()
	ACNox_EBase* OwnerEnemy;

#pragma region Update Anim
	UPROPERTY(BlueprintReadOnly, Category=Anim, meta=(AllowPrivateAccess=true))
	float Speed;
#pragma endregion

#pragma endregion
	
#pragma region Grenade
	UFUNCTION()
	void AnimNotify_Grenade() const;
#pragma endregion
	
#pragma region Attacking

#pragma endregion

#pragma region Heal
	const FName HealStartSection = "HealStart";
	const FName HealEndSection = "HealEnd";
#pragma endregion
	
#pragma region Zero
	UPROPERTY(BlueprintReadOnly, Category=Anim, meta=(AllowPrivateAccess=true))
	int32 IdleIdx = 0;
#pragma endregion

#pragma region Memory
	
#pragma region Attacking
	int8 AttackCombo = 0;
#pragma endregion

#pragma region Beam
	bool loopCheck = false;
	double LoopStartTime;
	UPROPERTY(EditDefaultsOnly)
	float MaxLoopDuration = 5.f;

	UFUNCTION()
	void AnimNotify_UsingBeamTimeChecker();
	UFUNCTION()
	void AnimNotify_BeamStart() const;
#pragma endregion

#pragma region Wave Pulse
	UFUNCTION()
	void AnimNotify_WavePulseStart() const;
#pragma endregion
	
#pragma endregion


};
