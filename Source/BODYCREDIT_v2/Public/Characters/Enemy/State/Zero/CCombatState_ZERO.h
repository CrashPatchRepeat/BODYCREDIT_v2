#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"

enum class ECombatState : uint8;
class ACNox_EBase;
class UCNox_FSMComp;

/**
 * 
 */
class BODYCREDIT_V2_API CCombatState_ZERO : public ICEStateStrategy
{
	
public:
	CCombatState_ZERO();
	virtual void Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp) override;

private:
	TMap<ECombatState, TSharedPtr<ICEStateStrategy>> CombatSubStrategies;
};
