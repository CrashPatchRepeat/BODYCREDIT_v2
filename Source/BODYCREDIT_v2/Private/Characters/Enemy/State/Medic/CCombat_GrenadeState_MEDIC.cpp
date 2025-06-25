#include "Characters/Enemy/State/Medic/CCombat_GrenadeState_MEDIC.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/CNox_Medic.h"
#include "Characters/Enemy/State/CEnemyState.h"
#include "Navigation/PathFollowingComponent.h"

void CCombat_GrenadeState_MEDIC::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;

	Owner->SetMovementSpeed(EEnemyMovementSpeed::Sprinting);
	EPathFollowingRequestResult::Type result = AICon->MoveToActor(Owner->GetTarget(), AcceptanceThreshold, true);
	if (result == EPathFollowingRequestResult::Failed)
	{
		AICon->StopMovement();
		return;
	}

	ACNox_Medic* Medic = Cast<ACNox_Medic>(Owner);
	if (!bFired)
	{
		bFired = true;
		Medic->HandleElectricGrenade();
	}
	else
	{
		if (!Medic->IsPlayingGrenade())
		{
			bFired = false;
			Owner->UsingSkill(ESkillCoolDown::Grenade);
			Owner->SetEnemyState(EEnemyState::Sense);
		}
	}
}
