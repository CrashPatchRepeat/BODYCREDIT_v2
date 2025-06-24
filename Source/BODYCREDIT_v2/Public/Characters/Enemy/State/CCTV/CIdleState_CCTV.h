#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"

/**
 * 
 */
class BODYCREDIT_V2_API CIdleState_CCTV : public ICEStateStrategy
{
public:
	CIdleState_CCTV(TUniquePtr<class CRotateMoveStrategy> InMoveStrategy);
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) override;

private:
	TUniquePtr<class CRotateMoveStrategy> MoveStrategy;
};
