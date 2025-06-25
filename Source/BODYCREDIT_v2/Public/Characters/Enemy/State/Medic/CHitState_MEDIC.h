#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"

class ACNox_EBase;
class UCNox_FSMComp;

/**
 * 
 */
class CHitState_MEDIC : public ICEStateStrategy
{	
public:
	virtual void Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp) override;
	
private:
	bool bFired = false;
};
