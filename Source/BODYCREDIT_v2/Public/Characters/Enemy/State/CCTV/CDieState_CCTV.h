#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"

class ACNox_EBase;
class UCFSMComponent;

/**
 * 
 */
class BODYCREDIT_V2_API CDieState_CCTV : public ICEStateStrategy
{
public:
	virtual void Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp) override;

private:
	bool bFired = false;
};
