#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CMoveStrategy.h"

class ACNox_EBase;

/**
 * 
 */
class CConditionalMoveStrategy_MEDIC : public ICMoveStrategy
{
public:
	virtual void Move(ACNox_EBase* Owner, float DeltaTime) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;
	
private:
	bool bIsMove = false;
	const float MeleeAttackRange = 200.f;
	const float GrenadeRange = 1500.f;

	float AcceptanceRadius = 10.f;
	float AcceptanceThreshold = 40.f;
	bool bMoving = false;

	void CovertToCombatState(ACNox_EBase* Owner);
};