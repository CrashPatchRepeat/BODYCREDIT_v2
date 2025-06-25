#include "Characters/Enemy/State/Memory/CCombatState_MEMORY.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/State/CEnemyState.h"
#include "Characters/Enemy/State/Memory/CCombat_BeamState_MEMORY.h"
#include "Characters/Enemy/State/Memory/CCombat_DefaultState_MEMORY.h"
#include "Characters/Enemy/State/Memory/CCombat_WavePulseState_MEMORY.h"
#include "Components/Enemy/CNox_FSMComp.h"

CCombatState_MEMORY::CCombatState_MEMORY()
{
	CombatSubStrategies.Add(ECombatState::Default, MakeShared<CCombat_DefaultState_MEMORY>());
	CombatSubStrategies.Add(ECombatState::Beam, MakeShared<CCombat_BeamState_MEMORY>());
	CombatSubStrategies.Add(ECombatState::WavePulse, MakeShared<CCombat_WavePulseState_MEMORY>());
}

void CCombatState_MEMORY::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	ECombatState SubState = FSMComp->GetCombatState();

	if (CombatSubStrategies.Contains(SubState))
	{
		CombatSubStrategies[SubState]->Execute(Owner, FSMComp);
	}

	Owner->UpdateSkillCoolDowns(ESkillCoolDown::Ranged, Owner->GetWorld()->GetDeltaSeconds());
	Owner->UpdateSkillCoolDowns(ESkillCoolDown::Beam, Owner->GetWorld()->GetDeltaSeconds());
	Owner->UpdateSkillCoolDowns(ESkillCoolDown::WavePulse, Owner->GetWorld()->GetDeltaSeconds());
}
