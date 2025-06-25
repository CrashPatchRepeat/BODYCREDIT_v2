#include "Characters/Enemy/CNox_Medic.h"

#include "NiagaraComponent.h"
#include "Components/Enemy/CNox_EAnimInstance.h"
#include "Components/Enemy/CNox_EStatusComp.h"

#pragma region Grenade
void ACNox_Medic::HandleElectricGrenade()
{
	// TODO Anim 추가 후 주석 해제
	// EnemyAnim->PlayGrenadeMontage();
}

bool ACNox_Medic::IsPlayingGrenade() const
{
	// TODO Anim 추가 후 주석 해제
	// return EnemyAnim->IsPlayingGrenade();
	return false;
}
#pragma endregion

#pragma region Heal
bool ACNox_Medic::IsLowHealth()
{
	return StatusComp->GetHealthPercent() <= HealStdValue;
}

void ACNox_Medic::HandleEquipShield(const bool bInEquipShield)
{
	// TODO Anim 추가 후 주석 해제
	// EnemyAnim->PlayShieldMontage(bInEquipShield);
	bInEquipShield ? HealEffect->Activate() : HealEffect->Deactivate();
}

bool ACNox_Medic::IsShielding() const
{
	// TODO Anim 추가 후 주석 해제
	// return EnemyAnim->IsShielding();
	return false;
}
#pragma endregion