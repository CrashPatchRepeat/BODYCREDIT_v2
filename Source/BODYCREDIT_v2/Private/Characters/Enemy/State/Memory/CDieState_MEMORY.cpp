#include "Characters/Enemy/State/Memory/CDieState_MEMORY.h"

#include "Characters/Enemy/CNox_EBase.h"

void CDieState_MEMORY::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	if (!bFired)
	{
		Owner->HandleDie();
		Owner->PlayDieSound();
		bFired = true;
	}
}
