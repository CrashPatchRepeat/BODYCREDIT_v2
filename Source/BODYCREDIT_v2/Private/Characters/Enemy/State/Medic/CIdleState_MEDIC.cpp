#include "Characters/Enemy/State/Medic/CIdleState_MEDIC.h"

#include "CRandomMoveStrategy_MEDIC.h"
#include "Characters/Enemy/CNox_EBase.h"

CIdleState_MEDIC::CIdleState_MEDIC(TUniquePtr<CRandomMoveStrategy_MEDIC> InMoveStrategy)
	: MoveStrategy(MoveTemp(InMoveStrategy)){}

void CIdleState_MEDIC::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	Owner->PlayIdleSound();	
	if (MoveStrategy) MoveStrategy->Move(Owner, Owner->GetWorld()->GetDeltaSeconds());
}
