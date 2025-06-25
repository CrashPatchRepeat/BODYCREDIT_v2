#include "Components/Enemy/CNox_EAnimInstance.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/CNox_Medic.h"
#include "Characters/Enemy/CNox_Memory.h"
#include "Characters/Enemy/CNox_Zero.h"

#pragma region Override
void UCNox_EAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
#pragma region Zero
	IdleIdx = FMath::RandRange(0, 8);
#pragma endregion

	OwnerEnemy = Cast<ACNox_EBase>(TryGetPawnOwner());
}

void UCNox_EAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!OwnerEnemy) return;
	{
		FVector velo = OwnerEnemy->GetVelocity();
		Speed = velo.Size();
	}
	
	if (loopCheck)
	{
		float Elapsed = GetWorld()->GetTimeSeconds() - LoopStartTime;
		// CLog::Print(FString::Printf(TEXT("Elapsed : %.2f"), Elapsed));
		if (Elapsed >= MaxLoopDuration)
		{
			loopCheck = false;
			Cast<ACNox_Memory>(OwnerEnemy)->BeamAttackEnd();
		}
	}
}
#pragma endregion

#pragma region Memory - Beam
void UCNox_EAnimInstance::PlayBeamAttack() const
{
	OwnerEnemy->PlayAnimMontage(OwnerEnemy->BeamMontage, 1.0f);
}

void UCNox_EAnimInstance::StopBeamAttack() const
{
	OwnerEnemy->StopAnimMontage(OwnerEnemy->BeamMontage);
}

bool UCNox_EAnimInstance::IsBeamAttacking() const
{
	return Montage_IsPlaying(OwnerEnemy->BeamMontage);
}

void UCNox_EAnimInstance::AnimNotify_UsingBeamTimeChecker()
{
	loopCheck = true;
	LoopStartTime = GetWorld()->GetTimeSeconds();
}

void UCNox_EAnimInstance::AnimNotify_BeamStart() const
{
	Cast<ACNox_Memory>(OwnerEnemy)->BeamAttack();
}
#pragma endregion

#pragma region Grenade
void UCNox_EAnimInstance::PlayGrenadeMontage() const
{
	OwnerEnemy->PlayAnimMontage(OwnerEnemy->GrenadeMontage, 1.0f);
}

bool UCNox_EAnimInstance::IsPlayingGrenade() const
{
	return Montage_IsPlaying(OwnerEnemy->GrenadeMontage);
}

void UCNox_EAnimInstance::AnimNotify_Grenade() const
{
	Cast<ACNox_Medic>(OwnerEnemy)->LaunchElectricGrenade();
}
#pragma endregion

#pragma region Attacking
void UCNox_EAnimInstance::PlayAttackMontage()
{
	if (OwnerEnemy->IsA(ACNox_Medic::StaticClass()) ||
		OwnerEnemy->IsA(ACNox_Zero::StaticClass()))
	{
		if (OwnerEnemy->AttackMontage) OwnerEnemy->PlayAnimMontage(OwnerEnemy->AttackMontage, 1.0f);
	}
	else if (OwnerEnemy->IsA(ACNox_Memory::StaticClass()))
	{
		if (OwnerEnemy->Attack1Montage)
		{
			AttackCombo = 1;
			OwnerEnemy->PlayAnimMontage(OwnerEnemy->Attack1Montage, 1.0f);
		}
	}
}

bool UCNox_EAnimInstance::IsAttacking() const
{
	if (OwnerEnemy->IsA(ACNox_Medic::StaticClass()) || OwnerEnemy->IsA(ACNox_Zero::StaticClass()))
	{
		if (OwnerEnemy->AttackMontage && Montage_IsPlaying(OwnerEnemy->AttackMontage)) return true;
		else return false;
	}
	else if (OwnerEnemy->IsA(ACNox_Memory::StaticClass()))
	{
		UAnimMontage* curMontage = OwnerEnemy->GetCurrentMontage();
		if (curMontage == OwnerEnemy->Attack1Montage || curMontage == OwnerEnemy->Attack2Montage
			|| curMontage == OwnerEnemy->Attack3Montage || curMontage == OwnerEnemy->Attack4Montage)
			return true;
		else return false;
	}
	return false;
}
#pragma endregion

#pragma region Heal
void UCNox_EAnimInstance::PlayHealMontage(const bool bInHealStart) const
{
	if (bInHealStart)
		OwnerEnemy->PlayAnimMontage(OwnerEnemy->HealMontage, 1.0f, HealStartSection);
	else
		OwnerEnemy->PlayAnimMontage(OwnerEnemy->HealMontage, -.65f, HealEndSection);
}

bool UCNox_EAnimInstance::IsHealing() const
{
	return Montage_IsPlaying(OwnerEnemy->HealMontage);
}
#pragma endregion

#pragma region Wave Pulse
void UCNox_EAnimInstance::PlayWavePulse() const
{
	OwnerEnemy->PlayAnimMontage(OwnerEnemy->WavePulseMontage, 1.0f);
}

bool UCNox_EAnimInstance::IsWavePulseAttacking() const
{
	return Montage_IsPlaying(OwnerEnemy->WavePulseMontage);
}

void UCNox_EAnimInstance::AnimNotify_WavePulseStart() const
{
	Cast<ACNox_Memory>(OwnerEnemy)->WavePulseAttack();
}
#pragma endregion

#pragma region Hit
void UCNox_EAnimInstance::PlayHitMontage(const int32 sectionIdx)
{
	OwnerEnemy->PlayAnimMontage(OwnerEnemy->HitMontage, 1, FName(FString::FromInt(sectionIdx)));
}

bool UCNox_EAnimInstance::IsHitting() const
{
	return Montage_IsPlaying(OwnerEnemy->HitMontage);
}
#pragma endregion
