#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"

enum class ECombatState : uint8;
class UCNox_FSMComp;
class ACNox_EBase;

/**
 * 
 */
class CCombatState_MEMORY : public ICEStateStrategy
{
public:
	CCombatState_MEMORY();
	virtual void Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp) override;

private:
	TMap<ECombatState, TSharedPtr<ICEStateStrategy>> CombatSubStrategies;
};
