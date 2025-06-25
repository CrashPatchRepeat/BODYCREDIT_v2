#include "Characters/Enemy/State/Memory/CCombat_BeamState_MEMORY.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/CNox_Memory.h"
#include "Characters/Enemy/State/CEnemyState.h"

void CCombat_BeamState_MEMORY::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;
	AICon->StopMovement();
	
	ACNox_Memory* MemoryCollector = Cast<ACNox_Memory>(Owner);
	if (!bFired)
	{
		MemoryCollector->ShutBeam();
		bFired = true;
	}
	else
	{
		if (!MemoryCollector->IsPlayBeam())
		{
			Owner->UsingSkill(ESkillCoolDown::Beam);
			Owner->SetEnemyState(EEnemyState::Sense);
			bFired = false;
		}
	}
}
