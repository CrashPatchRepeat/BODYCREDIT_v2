#include "Characters/Enemy/State/Zero/CDieState_ZERO.h"

#include "Characters/Enemy/CNox_EBase.h"

void CDieState_ZERO::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	if (!bFired)
	{
		Owner->HandleDie();
		bFired = true;
	}
}
