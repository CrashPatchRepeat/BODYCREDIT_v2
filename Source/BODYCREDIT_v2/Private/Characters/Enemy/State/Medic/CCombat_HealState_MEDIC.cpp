#include "Characters/Enemy/State/Medic/CCombat_HealState_MEDIC.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/CNox_Medic.h"
#include "Characters/Enemy/State/CEnemyState.h"

void CCombat_HealState_MEDIC::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	ACNox_Medic* Medic = Cast<ACNox_Medic>(Owner);

	Owner->SetMovementSpeed(EEnemyMovementSpeed::Idle);
	if (!bHealEnd)	// 힐이 끝나지 않았을 때
	{
		if (!bFired) // 힐 애니메이션이 실행되지 않았을 때
		{
			AAIController* AICon = Cast<AAIController>(Owner->GetController());
			if (!AICon) return;
			AICon->StopMovement();
			
			bFired = true;
			Medic->HandleHeal(bFired);
		}
		else // 힐 사용 중
		{
			CurHealTime += Medic->GetWorld()->GetDeltaSeconds();
			if (CurHealTime >= HealTime) // 힐 유지 시간 종료
			{
				bFired = false;
				CurHealTime = 0.f;
				bHealEnd = true;
				Medic->HandleHeal(bFired);
				return;
			}

			Owner->HealHP();
		}
	}
	else
	{
		if (!Medic->IsHealing())
		{
			bHealEnd = false;
			Owner->UsingSkill(ESkillCoolDown::Heal);
			Owner->SetEnemyState(EEnemyState::Sense);
		}
	}
}
