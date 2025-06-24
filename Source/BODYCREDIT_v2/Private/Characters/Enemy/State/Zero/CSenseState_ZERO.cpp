#include "Characters/Enemy/State/Zero/CSenseState_ZERO.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/State/Zero/CConditionalMoveStrategy_ZERO.h"

CSenseState_ZERO::CSenseState_ZERO(TUniquePtr<CConditionalMoveStrategy_ZERO> InMoveStrategy)
	: MoveStrategy(MoveTemp(InMoveStrategy)){}

void CSenseState_ZERO::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	Owner->PlaySenseSound();
	if (MoveStrategy) MoveStrategy->Move(Owner, Owner->GetWorld()->GetDeltaSeconds());
}

void CSenseState_ZERO::ResetVal(ACNox_EBase* Owner)
{
	if (MoveStrategy) MoveStrategy->ResetVal(Owner);
}
