#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CMoveStrategy.generated.h"

class ACNox_EBase;

UINTERFACE(MinimalAPI)
class UCMoveStrategy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BODYCREDIT_V2_API ICMoveStrategy
{
	GENERATED_BODY()

public:
	virtual void Move(ACNox_EBase* Owner, float DeltaTime);
	virtual void ResetVal(ACNox_EBase* Owner);;
};
