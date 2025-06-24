#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CMoveStrategy.h"

/**
 * 
 */
class BODYCREDIT_V2_API CConditionalMoveStrategy_ZERO: public ICMoveStrategy
{
public:
	virtual void Move(ACNox_EBase* Owner, float DeltaTime) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;
	
private:
	bool bIsMove = false;
	float AcceptanceRadius = 10.f;
	float AcceptanceThreshold = 40.f;
	const float MeleeAttackRange = 100.f;

	void CovertToCombatState(ACNox_EBase* Owner);
};
