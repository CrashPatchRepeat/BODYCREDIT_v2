#include "Characters/Enemy/State/Memory/CHitState_MEMORY.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/State/CEnemyState.h"

void CHitState_MEMORY::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	if (!bFired)
	{
		Owner->HandleHit(FMath::RandRange(1, 4));
		Owner->PlayHitSound();
		bFired = true;
	}
	else
	{
		if (!Owner->IsHitting())
		{
			bFired = false;
			Owner->SetEnemyState(EEnemyState::Sense);
		}
	}
}
