#include "Characters/Enemy/State/Memory/CCombat_DefaultState_MEMORY.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/State/CEnemyState.h"
#include "Navigation/PathFollowingComponent.h"

void CCombat_DefaultState_MEMORY::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
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
	
	if (!bFired)
	{
		Owner->HandleAttack();
		bFired = true;
	}
	else
	{
		if (!Owner->IsAttacking())
		{
			Owner->UsingSkill(ESkillCoolDown::Ranged);
			Owner->SetEnemyState(EEnemyState::Sense);
			bFired = false;
		}
	}
}
