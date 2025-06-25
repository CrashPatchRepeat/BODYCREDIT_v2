#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"

class CMemoryMoveStrategy;
class ACNox_EBase;
class UCNox_FSMComp;

/**
 * 
 */
class BODYCREDIT_V2_API CMemoryHuntState_MEMORY : public ICEStateStrategy
{
public:
	CMemoryHuntState_MEMORY(TUniquePtr<CMemoryMoveStrategy> InMoveStrategy);
	virtual void Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp) override;
	
private:
	TUniquePtr<CMemoryMoveStrategy> MoveStrategy;
};
