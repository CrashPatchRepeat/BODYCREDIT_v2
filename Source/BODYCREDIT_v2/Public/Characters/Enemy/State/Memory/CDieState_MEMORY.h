#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"

class UCNox_FSMComp;
class ACNox_EBase;

/**
 * 
 */
class BODYCREDIT_V2_API CDieState_MEMORY : public ICEStateStrategy
{
public:
	virtual void Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp) override;

private:
	bool bFired = false;
};
