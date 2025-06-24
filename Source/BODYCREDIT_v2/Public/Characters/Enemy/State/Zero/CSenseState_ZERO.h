#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"

class CConditionalMoveStrategy_ZERO;
class ACNox_EBase;
class UCNox_FSMComp;

/**
 * 
 */
class BODYCREDIT_V2_API CSenseState_ZERO : public ICEStateStrategy
{
private:
	TUniquePtr<CConditionalMoveStrategy_ZERO> MoveStrategy;
	
public:
	CSenseState_ZERO(TUniquePtr<CConditionalMoveStrategy_ZERO> InMoveStrategy);
	virtual void Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;
};
