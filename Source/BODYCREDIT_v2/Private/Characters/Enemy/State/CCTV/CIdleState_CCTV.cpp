#include "Characters/Enemy/State/CCTV/CIdleState_CCTV.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/State/CCTV/CRotateMoveStrategy.h"

CIdleState_CCTV::CIdleState_CCTV(TUniquePtr<CRotateMoveStrategy> InMoveStrategy)
	: MoveStrategy(MoveTemp(InMoveStrategy)){ }

void CIdleState_CCTV::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	if (MoveStrategy) MoveStrategy->Move(Owner, Owner->GetWorld()->GetDeltaSeconds());
}
