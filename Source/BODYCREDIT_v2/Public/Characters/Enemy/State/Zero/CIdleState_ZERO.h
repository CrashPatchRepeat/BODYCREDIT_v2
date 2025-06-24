#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"

class CRandomMoveStrategy_ZERO;
class ACNox_EBase;
class UCNox_FSMComp;

/**
 * 
 */
class BODYCREDIT_V2_API CIdleState_ZERO : public ICEStateStrategy
{
private:
	TUniquePtr<CRandomMoveStrategy_ZERO> MoveStrategy;
	
public:
	CIdleState_ZERO(TUniquePtr<CRandomMoveStrategy_ZERO> InMoveStrategy);
	virtual void Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;
};
