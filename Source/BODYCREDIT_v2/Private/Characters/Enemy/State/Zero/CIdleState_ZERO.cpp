#include "Characters/Enemy/State/Zero/CIdleState_ZERO.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/State/Zero/CRandomMoveStrategy_ZERO.h"

CIdleState_ZERO::CIdleState_ZERO(TUniquePtr<CRandomMoveStrategy_ZERO> InMoveStrategy)
	: MoveStrategy(MoveTemp(InMoveStrategy)){}

void CIdleState_ZERO::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	Owner->PlayIdleSound();
	if (MoveStrategy) MoveStrategy->Move(Owner, Owner->GetWorld()->GetDeltaSeconds());
}

void CIdleState_ZERO::ResetVal(ACNox_EBase* Owner)
{
	if (MoveStrategy) MoveStrategy->ResetVal(Owner);
}
