#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"

class CRandomMoveStrategy_Memory;
class ACNox_EBase;
class UCNox_FSMComp;

/**
 * 
 */
class CIdleState_MEMORY : public ICEStateStrategy
{
public:
	CIdleState_MEMORY(TUniquePtr<CRandomMoveStrategy_Memory> InMoveStrategy);
	virtual void Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp) override;

private:
	TUniquePtr<CRandomMoveStrategy_Memory> MoveStrategy;

	bool HavingMemory(ACNox_EBase* Owner);
	bool MemoryCheck(ACNox_EBase* Owner);
};
