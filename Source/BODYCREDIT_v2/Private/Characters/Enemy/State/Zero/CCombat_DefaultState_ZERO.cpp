#include "Characters/Enemy/State/Zero/CCombat_DefaultState_ZERO.h"

#include "Characters/Enemy/State/Zero/CDefaultAttackStrategy_ZERO.h"

CCombat_DefaultState_ZERO::CCombat_DefaultState_ZERO(TUniquePtr<CDefaultAttackStrategy_ZERO> InAttackStrategy)
	: AttackStrategy(MoveTemp(InAttackStrategy)){}

void CCombat_DefaultState_ZERO::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	if (AttackStrategy) AttackStrategy->Execute(Owner, FSMComp);
}
