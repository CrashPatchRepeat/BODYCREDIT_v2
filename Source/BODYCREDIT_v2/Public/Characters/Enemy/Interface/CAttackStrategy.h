#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CAttackStrategy.generated.h"

class ACNox_EBase;
class UCNox_FSMComp;

UINTERFACE(MinimalAPI)
class UCAttackStrategy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BODYCREDIT_V2_API ICAttackStrategy
{
	GENERATED_BODY()

public:
	virtual void Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp) = 0;
	virtual void ResetVal(ACNox_EBase* Owner) {};
};
