#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"

class CConditionalMoveStrategy_MEDIC;
class ACNox_EBase;
class UCNox_FSMComp;

/**
 * 
 */
class CSenseState_MEDIC : public ICEStateStrategy
{
public:
	CSenseState_MEDIC(TUniquePtr<CConditionalMoveStrategy_MEDIC> InMoveStrategy);
	virtual void Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;
	
private:
	TUniquePtr<CConditionalMoveStrategy_MEDIC> MoveStrategy;
};
