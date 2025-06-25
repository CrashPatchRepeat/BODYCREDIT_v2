#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"

/**
 * 
 */
class BODYCREDIT_V2_API CCombat_DefaultState_MEMORY : public ICEStateStrategy
{
public:
	virtual void Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp) override;
	
private:
	bool bFired = false;
	float AcceptanceThreshold = 500.f;	
};