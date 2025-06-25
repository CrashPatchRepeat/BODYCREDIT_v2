#include "Characters/Enemy/State/Medic/CDieState_MEDIC.h"

#include "Characters/Enemy/CNox_EBase.h"

void CDieState_MEDIC::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	if (!bFired)
	{
		Owner->HandleDie();
		Owner->PlayDieSound();
		bFired = true;
	}
}
