#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"

class ACNox_EBase;
class UCNox_FSMComp;
class CRotateMoveStrategy;

/**
 * 
 */
class BODYCREDIT_V2_API CIdleState_CCTV : public ICEStateStrategy
{
public:
	CIdleState_CCTV(TUniquePtr<CRotateMoveStrategy> InMoveStrategy);
	virtual void Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp) override;

private:
	TUniquePtr<CRotateMoveStrategy> MoveStrategy;
};
