#include "Characters/Enemy/State/Memory/CSenseState_MEMORY.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/State/CEnemyState.h"
#include "Characters/Enemy/State/Memory/CConditionalMoveStrategy_MEMORY.h"

CSenseState_MEMORY::CSenseState_MEMORY(TUniquePtr<CConditionalMoveStrategy_MEMORY> InMoveStrategy)
	: MoveStrategy(MoveTemp(InMoveStrategy)){}

void CSenseState_MEMORY::Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp)
{
	Owner->PlaySenseSound();
	if (MoveStrategy) MoveStrategy->Move(Owner, Owner->GetWorld()->GetDeltaSeconds());

	Owner->UpdateSkillCoolDowns(ESkillCoolDown::Ranged, Owner->GetWorld()->GetDeltaSeconds());
	Owner->UpdateSkillCoolDowns(ESkillCoolDown::Beam, Owner->GetWorld()->GetDeltaSeconds());
	Owner->UpdateSkillCoolDowns(ESkillCoolDown::WavePulse, Owner->GetWorld()->GetDeltaSeconds());
}

void CSenseState_MEMORY::ResetVal(ACNox_EBase* Owner)
{
	if (MoveStrategy) MoveStrategy->ResetVal(Owner);
}
