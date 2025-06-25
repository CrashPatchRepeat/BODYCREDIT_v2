#include "Characters/Enemy/State/Memory/CCombat_WavePulseState_MEMORY.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/CNox_Memory.h"
#include "Characters/Enemy/State/CEnemyState.h"

void CCombat_WavePulseState_MEMORY::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;
	AICon->StopMovement();
	
	ACNox_Memory* MemoryCollector = Cast<ACNox_Memory>(Owner);
	if (!bFired)
	{
		MemoryCollector->ShutWavePulse();
		bFired = true;
	}
	else
	{
		if (!MemoryCollector->IsPlayWavePulse())
		{
			Owner->UsingSkill(ESkillCoolDown::WavePulse);
			Owner->SetEnemyState(EEnemyState::Sense);
			bFired = false;
		}
	}
}
