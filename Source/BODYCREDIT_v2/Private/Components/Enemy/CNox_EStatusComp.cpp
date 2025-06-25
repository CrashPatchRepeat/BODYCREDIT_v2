#include "Components/Enemy/CNox_EStatusComp.h"

void UCNox_EStatusComp::HealHP(float InHealAmount)
{
	SetHealth(FMath::Min(GetMaxHealth(), GetHealth() + InHealAmount));
}

