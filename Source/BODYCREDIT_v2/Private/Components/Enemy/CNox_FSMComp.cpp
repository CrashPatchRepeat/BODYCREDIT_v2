#include "Components/Enemy/CNox_FSMComp.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Global.h"
#include "Characters/Enemy/CNox_Medic.h"
#include "Characters/Enemy/CNox_Memory.h"
#include "Characters/Enemy/CNox_Zero.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"
#include "Characters/Enemy/State/CCTV/CDieState_CCTV.h"
#include "Characters/Enemy/State/CCTV/CIdleState_CCTV.h"
#include "Characters/Enemy/State/CCTV/CRotateMoveStrategy.h"
#include "Characters/Enemy/State/Zero/CCombatState_ZERO.h"
#include "Characters/Enemy/State/Zero/CConditionalMoveStrategy_ZERO.h"
#include "Characters/Enemy/State/Zero/CDieState_ZERO.h"
#include "Characters/Enemy/State/Zero/CHitState_ZERO.h"
#include "Characters/Enemy/State/Zero/CIdleState_ZERO.h"
#include "Characters/Enemy/State/Zero/CRandomMoveStrategy_ZERO.h"
#include "Characters/Enemy/State/Zero/CSenseState_ZERO.h"

#pragma region 생성자
UCNox_FSMComp::UCNox_FSMComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}
#pragma endregion

#pragma region 상태_관리
void UCNox_FSMComp::UpdateState()
{
	if (EnemyStrategies.Contains(CurrentEnemyState)) EnemyStrategies[CurrentEnemyState]->Execute(OwnerEnemy, this);
}

void UCNox_FSMComp::ResetVal(EEnemyType Type)
{
	switch (Type)
	{
	case EEnemyType::Zero:
		if (CurrentEnemyState == EEnemyState::Hit)
		{
			EnemyStrategies[EEnemyState::IDLE]->ResetVal(OwnerEnemy);
			EnemyStrategies[EEnemyState::Sense]->ResetVal(OwnerEnemy);
			EnemyStrategies[EEnemyState::Combat]->ResetVal(OwnerEnemy);
		}
		else if (CurrentEnemyState == EEnemyState::Combat)
		{
			EnemyStrategies[EEnemyState::IDLE]->ResetVal(OwnerEnemy);
			EnemyStrategies[EEnemyState::Sense]->ResetVal(OwnerEnemy);
		}
		else if (CurrentEnemyState == EEnemyState::Sense)
		{
			EnemyStrategies[EEnemyState::IDLE]->ResetVal(OwnerEnemy);
		}
		break;
	case EEnemyType::MedicAndroid:
	case EEnemyType::MemoryCollector:
		EnemyStrategies[EEnemyState::Sense]->ResetVal(OwnerEnemy);
		break;
	}
}

void UCNox_FSMComp::SetEnemyState(EEnemyState NewState)
{
	CurrentEnemyState = NewState;
}

void UCNox_FSMComp::SetCombatState(ECombatState NewCombatState)
{
	CurrentCombatState = NewCombatState;
}
#pragma endregion

#pragma region 스킬_쿨타임_관리
FName UCNox_FSMComp::GetSkillName(ESkillCoolDown SkillType) const
{
	switch (SkillType)
	{
	case ESkillCoolDown::Melee: return FName(TEXT("MeleeCoolDown"));
	case ESkillCoolDown::Ranged: return FName(TEXT("RangedCoolDown"));
	case ESkillCoolDown::Heal: return FName(TEXT("HealCoolDown"));
	case ESkillCoolDown::Grenade: return FName(TEXT("GrenadeCoolDown"));
	case ESkillCoolDown::Beam: return FName(TEXT("BeamCoolDown"));
	case ESkillCoolDown::WavePulse: return FName(TEXT("WavePulseCoolDown"));
	default: return FName(TEXT("UnknownCoolDown"));
	}
}
#pragma endregion

#pragma region UE_오버라이드
void UCNox_FSMComp::BeginPlay()
{
	Super::BeginPlay();
	
	EnemyStrategies = CreateStrategies(OwnerEnemy->GetEnemyType());
	InitSkillCoolDowns(OwnerEnemy->GetEnemyType());
}
#pragma endregion

#pragma region 내부_상태
TMap<EEnemyState, TSharedPtr<ICEStateStrategy>> UCNox_FSMComp::CreateStrategies(EEnemyType Type)
{
	TMap<EEnemyState, TSharedPtr<ICEStateStrategy>> Result;
	// TODO: 상태별 전략 객체 생성 및 등록
	switch (Type)
	{
	case EEnemyType::Cctv:
		{
			{
				TUniquePtr<CRotateMoveStrategy> MoveStrategy = MakeUnique<CRotateMoveStrategy>();
				Result.Add(EEnemyState::IDLE, MakeShared<CIdleState_CCTV>(MoveTemp(MoveStrategy)));
			}
			Result.Add(EEnemyState::Die, MakeShared<CDieState_CCTV>());
		}
		break;
	case EEnemyType::Zero:
		{
			{
				TUniquePtr<CRandomMoveStrategy_ZERO> MoveStrategy = MakeUnique<CRandomMoveStrategy_ZERO>();
				Result.Add(EEnemyState::IDLE, MakeShared<CIdleState_ZERO>(MoveTemp(MoveStrategy)));
			}
			{
				TUniquePtr<CConditionalMoveStrategy_ZERO> ConditionalMove = MakeUnique<CConditionalMoveStrategy_ZERO>();
				Result.Add(EEnemyState::Sense, MakeShared<CSenseState_ZERO>(MoveTemp(ConditionalMove)));
			}
			Result.Add(EEnemyState::Hit, MakeShared<CHitState_ZERO>());
			Result.Add(EEnemyState::Combat, MakeShared<CCombatState_ZERO>());
			Result.Add(EEnemyState::Die, MakeShared<CDieState_ZERO>());
		}
		break;
	case EEnemyType::MedicAndroid:
		// {
		// 	{
		// 		TUniquePtr<CRandomMoveStrategy> MoveStrategy = MakeUnique<CRandomMoveStrategy>();
		// 		Result.Add(EEnemyState::IDLE, MakeShared<CIdleState_MEDIC>(MoveTemp(MoveStrategy)));
		// 	}
		// 	{
		// 		TUniquePtr<CConditionalMoveStrategy_MEDIC> ConditionalMove = MakeUnique<
		// 			CConditionalMoveStrategy_MEDIC>();
		// 		Result.Add(EEnemyState::Sense, MakeShared<CSenseState_MEDIC>(MoveTemp(ConditionalMove)));
		// 	}
		// 	Result.Add(EEnemyState::Hit, MakeShared<CHitState_MEDIC>());
		// 	Result.Add(EEnemyState::Combat, MakeShared<CCombatState_MEDIC>());
		// 	Result.Add(EEnemyState::Die, MakeShared<CDieState_MEDIC>());
		// }
		break;
	case EEnemyType::MemoryCollector:
		// {
		// 	{
		// 		TUniquePtr<CRandomMoveStrategy_Memory> MoveStrategy = MakeUnique<CRandomMoveStrategy_Memory>();
		// 		Result.Add(EEnemyState::IDLE, MakeShared<CIdleState_MEMORY>(MoveTemp(MoveStrategy)));
		// 	}
		// 	{
		// 		TUniquePtr<CConditionalMoveStrategy_MEMORY> ConditionalMove = MakeUnique<CConditionalMoveStrategy_MEMORY>();
		// 		Result.Add(EEnemyState::Sense, MakeShared<CSenseState_MEMORY>(MoveTemp(ConditionalMove)));
		// 	}
		// 	{
		// 		TUniquePtr<CMemoryMoveStrategy> MemoryMove = MakeUnique<CMemoryMoveStrategy>();
		// 		Result.Add(EEnemyState::MemoryHunt, MakeShared<CMemoryHuntState_MEMORY>(MoveTemp(MemoryMove)));
		// 	}
		// 	Result.Add(EEnemyState::Combat, MakeShared<CCombatState_MEMORY>());
		// 	Result.Add(EEnemyState::Hit, MakeShared<CHitState_MEMORY>());
		// 	Result.Add(EEnemyState::Die, MakeShared<CDieState_MEMORY>());
		// }		
		break;
	}
	return Result;
}

void UCNox_FSMComp::InitSkillCoolDowns(EEnemyType Type)
{
	// TODO: 스킬 쿨타임 초기화 로직 구현
	switch (Type)
	{
	case EEnemyType::Zero:
		{
			ACNox_Zero* ZeroEnemy = Cast<ACNox_Zero>(OwnerEnemy);
			if (ZeroEnemy)
			{
				SkillCoolDowns.Add(GetSkillName(ESkillCoolDown::Melee), 0.f);
				SkillMaxCoolDowns.Add(GetSkillName(ESkillCoolDown::Melee), ZeroEnemy->MeleeCoolDown);
			}
		}
		break;
	case EEnemyType::MedicAndroid:
		// {
		// 	ACNox_Medic* MedicEnemy = Cast<ACNox_Medic>(OwnerEnemy);
		// 	if (MedicEnemy)
		// 	{
		// 		SkillCoolDowns.Add(GetSkillName(ESkillCoolDown::Melee), 0.f);
		// 		SkillMaxCoolDowns.Add(GetSkillName(ESkillCoolDown::Melee), MedicEnemy->MeleeCoolDown);
		// 		SkillCoolDowns.Add(GetSkillName(ESkillCoolDown::Heal), 0.f);
		// 		SkillMaxCoolDowns.Add(GetSkillName(ESkillCoolDown::Heal), MedicEnemy->HealCoolDown);
		// 		SkillCoolDowns.Add(GetSkillName(ESkillCoolDown::Grenade), 0.f);
		// 		SkillMaxCoolDowns.Add(GetSkillName(ESkillCoolDown::Grenade), MedicEnemy->GrenadeCoolDown);
		// 	}
		// }
		break;
	default:
		// {
		// 	ACNox_Memory* MemoryEnemy = Cast<ACNox_Memory>(OwnerEnemy);
		// 	if (MemoryEnemy)
		// 	{
		// 		SkillCoolDowns.Add(GetSkillName(ESkillCoolDown::Ranged), 0.f);
		// 		SkillMaxCoolDowns.Add(GetSkillName(ESkillCoolDown::Ranged), MemoryEnemy->RangedCoolDown);
		// 		SkillCoolDowns.Add(GetSkillName(ESkillCoolDown::Beam), 0.f);
		// 		SkillMaxCoolDowns.Add(GetSkillName(ESkillCoolDown::Beam), MemoryEnemy->BeamCoolDown);
		// 		SkillCoolDowns.Add(GetSkillName(ESkillCoolDown::WavePulse), 0.f);
		// 		SkillMaxCoolDowns.Add(GetSkillName(ESkillCoolDown::WavePulse), MemoryEnemy->WavePulseCoolDown);
		// 	}
		// }
		break;
	}
}
#pragma endregion

