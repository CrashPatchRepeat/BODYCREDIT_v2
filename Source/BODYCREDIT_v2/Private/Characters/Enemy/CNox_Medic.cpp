#include "Characters/Enemy/CNox_Medic.h"

#include "NiagaraComponent.h"
#include "Components/Enemy/CNox_EAnimInstance.h"
#include "Components/Enemy/CNox_EStatusComp.h"

#pragma region Grenade
void ACNox_Medic::HandleElectricGrenade()
{
	EnemyAnim->PlayGrenadeMontage();
}

bool ACNox_Medic::IsPlayingGrenade() const
{
	return EnemyAnim->IsPlayingGrenade();
}

void ACNox_Medic::LaunchElectricGrenade()
{
	FVector startLoc = GetMesh()->GetSocketLocation(FName("GrenadeSocket"));
	FVector targetLoc = Target->GetActorLocation();
	FVector outVelocity;
	SuggestProjectileVelocityWithLimit(outVelocity, this->GetActorLocation(), targetLoc);
	// TODO Grenade 추가 후 주석 해제
	// if (ElectricGrenade) ElectricGrenade->InitializeGrenade(startLoc, targetLoc, outVelocity);
}
#pragma endregion

#pragma region Heal
bool ACNox_Medic::IsLowHealth()
{
	return StatusComp->GetHealthPercent() <= HealStdValue;
}

void ACNox_Medic::HandleHeal(const bool bInEquipShield)
{
	EnemyAnim->PlayHealMontage(bInEquipShield);
	bInEquipShield ? HealEffect->Activate() : HealEffect->Deactivate();
}

bool ACNox_Medic::IsHealing() const
{
	return EnemyAnim->IsHealing();
}

void ACNox_Medic::SuggestProjectileVelocityWithLimit(FVector& OutVelocity, const FVector& StartLocation,
	const FVector& TargetLocation, float MaxSpeed, float GravityZ)
{
	const FVector Delta = TargetLocation - StartLocation;
	FVector DeltaXY = FVector(Delta.X, Delta.Y, 0.f);
	float HorizontalDistance = DeltaXY.Size();
	float DeltaZ = Delta.Z;

	float ArcHeight = FMath::Clamp(HorizontalDistance * .2f, 100.f, 300.f);

	float Vz = FMath::Sqrt(2 * FMath::Abs(GravityZ) * ArcHeight);

	float TimeUp = Vz / FMath::Abs(GravityZ);
	float TimeDown = FMath::Sqrt(FMath::Max(2 * (ArcHeight - DeltaZ), 0.f) / FMath::Abs(GravityZ));
	float TotalTime = TimeUp + TimeDown;

	// 최소 시간 보정 (너무 가까울 때 처리)
	if (TotalTime <= 0.f) TotalTime = 0.1f;

	FVector DirXY = DeltaXY.GetSafeNormal();
	float Vxy = HorizontalDistance / TotalTime;

	FVector Velocity = DirXY * Vxy + FVector(0, 0, Vz);
	float InitialSpeed = Velocity.Size();

	if (InitialSpeed > MaxSpeed)
	{
		float Scale = MaxSpeed / InitialSpeed;
		Velocity *= Scale;
	}

	OutVelocity = Velocity;
}
#pragma endregion
