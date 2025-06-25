#include "Characters/Enemy/State/Memory/CMemoryHuntState_MEMORY.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/State/Memory/CMemoryMoveStrategy.h"

CMemoryHuntState_MEMORY::CMemoryHuntState_MEMORY(TUniquePtr<CMemoryMoveStrategy> InMoveStrategy)
	: MoveStrategy(MoveTemp(InMoveStrategy)){}

void CMemoryHuntState_MEMORY::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	if (MoveStrategy) MoveStrategy->Move(Owner, Owner->GetWorld()->GetDeltaSeconds());
}
