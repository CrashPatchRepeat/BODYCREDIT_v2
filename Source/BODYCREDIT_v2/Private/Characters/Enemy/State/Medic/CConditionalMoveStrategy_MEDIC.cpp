#include "Characters/Enemy/State/Medic/CConditionalMoveStrategy_MEDIC.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/CNox_Medic.h"
#include "Characters/Enemy/State/CEnemyState.h"
#include "Navigation/PathFollowingComponent.h"

void CConditionalMoveStrategy_MEDIC::Move(ACNox_EBase* Owner, float DeltaTime)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;

	Owner->SetMovementSpeed(EEnemyMovementSpeed::Sprinting);
	EPathFollowingRequestResult::Type result = AICon->MoveToActor(Owner->GetTarget(), AcceptanceThreshold, true);
	switch (result)
	{
	case EPathFollowingRequestResult::AlreadyAtGoal:
		break;
	default:
		bIsMove = true;
		break;
	}
	CovertToCombatState(Owner); // 공격 쿨타임이 지났으면 공격 상태로 전환
}

void CConditionalMoveStrategy_MEDIC::ResetVal(ACNox_EBase* Owner)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;
	AICon->StopMovement();
}

void CConditionalMoveStrategy_MEDIC::CovertToCombatState(ACNox_EBase* Owner)
{
	// 플레이어가 공격사거리에 있는지 확인
	if (Owner->IsSkillReady(ESkillCoolDown::Melee) && Owner->IsPlayerInForwardDegree(MeleeAttackRange))
	{
		if (!bIsMove) Owner->SetRotateToTarget();
			
		Owner->SetCombatState(ECombatState::Default);
		Owner->SetEnemyState(EEnemyState::Combat);
	}
	// 플레이어가 섬광탄사거리에 있는지 확인
	else if (Owner->IsSkillReady(ESkillCoolDown::Grenade) && Owner->IsPlayerInForwardDegree(GrenadeRange))
	{
		Owner->SetCombatState(ECombatState::Grenade);
		Owner->SetEnemyState(EEnemyState::Combat);
	}
	// 체력이 낮은지 확인
	else if (Owner->IsSkillReady(ESkillCoolDown::Heal) && Cast<ACNox_Medic>(Owner)->IsLowHealth())
	{
		Owner->SetCombatState(ECombatState::Heal);
		Owner->SetEnemyState(EEnemyState::Combat);
	}

	bIsMove = false;
}
