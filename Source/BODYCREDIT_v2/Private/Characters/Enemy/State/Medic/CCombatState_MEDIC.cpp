#include "Characters/Enemy/State/Medic/CCombatState_MEDIC.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/State/CEnemyState.h"
#include "Characters/Enemy/State/Medic/CCombat_DefaultState_MEDIC.h"
#include "Characters/Enemy/State/Medic/CCombat_GrenadeState_MEDIC.h"
#include "Characters/Enemy/State/Medic/CCombat_HealState_MEDIC.h"
#include "Components/Enemy/CNox_FSMComp.h"

CCombatState_MEDIC::CCombatState_MEDIC()
{
	CombatSubStrategies.Add(ECombatState::Default, MakeShared<CCombat_DefaultState_MEDIC>());
	CombatSubStrategies.Add(ECombatState::Grenade, MakeShared<CCombat_GrenadeState_MEDIC>());
	CombatSubStrategies.Add(ECombatState::Heal, MakeShared<CCombat_HealState_MEDIC>());
}

void CCombatState_MEDIC::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	ECombatState SubState = FSMComp->GetCombatState();

	if (CombatSubStrategies.Contains(SubState))
	{
		CombatSubStrategies[SubState]->Execute(Owner, FSMComp);
	}

	Owner->UpdateSkillCoolDowns(ESkillCoolDown::Melee, Owner->GetWorld()->GetDeltaSeconds());
	Owner->UpdateSkillCoolDowns(ESkillCoolDown::Heal, Owner->GetWorld()->GetDeltaSeconds());
	Owner->UpdateSkillCoolDowns(ESkillCoolDown::Grenade, Owner->GetWorld()->GetDeltaSeconds());
}
