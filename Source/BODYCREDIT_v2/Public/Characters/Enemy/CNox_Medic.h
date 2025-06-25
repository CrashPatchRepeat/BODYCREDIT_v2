#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "CNox_Medic.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API ACNox_Medic : public ACNox_EBase
{
	GENERATED_BODY()

public:
#pragma region Grenade
	void HandleElectricGrenade();
	bool IsPlayingGrenade() const;
#pragma endregion

#pragma region Heal
	bool IsLowHealth();
	void HandleEquipShield(bool bInEquipShield);
	bool IsShielding() const;
#pragma endregion

#pragma region Skill CoolDown
	UPROPERTY(EditDefaultsOnly, Category = "Skill CoolDown")
	float MeleeCoolDown = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Skill CoolDown")
	float HealCoolDown = 20.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Skill CoolDown")
	float GrenadeCoolDown = 10.0f;
#pragma endregion
	
private:
#pragma region Heal
	UPROPERTY(EditDefaultsOnly, Category=Heal)
	float HealStdValue = 0.4f;
	UPROPERTY(EditDefaultsOnly, Category=Heal)
	class UNiagaraSystem* HealEffectFactory = nullptr;
	UPROPERTY()
	class UNiagaraComponent* HealEffect = nullptr;
#pragma endregion
};
