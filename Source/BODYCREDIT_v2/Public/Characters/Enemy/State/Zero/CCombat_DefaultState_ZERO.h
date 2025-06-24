#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"

class CDefaultAttackStrategy_ZERO;
class ACNox_EBase;
class UCNox_FSMComp;

/**
 * 
 */
class BODYCREDIT_V2_API CCombat_DefaultState_ZERO : public ICEStateStrategy
{
public:
	CCombat_DefaultState_ZERO(TUniquePtr<CDefaultAttackStrategy_ZERO> InAttackStrategy);
	virtual void Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp) override;
	
private:
	TUniquePtr<CDefaultAttackStrategy_ZERO> AttackStrategy;
};
