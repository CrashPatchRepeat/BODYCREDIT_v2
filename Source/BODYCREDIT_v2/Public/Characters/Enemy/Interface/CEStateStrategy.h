#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CEStateStrategy.generated.h"

class ACNox_EBase;
class UCFSMComponent;

UINTERFACE(MinimalAPI)
class UCEStateStrategy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BODYCREDIT_V2_API ICEStateStrategy
{
	GENERATED_BODY()

public:
	virtual void Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp) = 0;
	virtual void ResetVal(ACNox_EBase* Owner) {};
};
