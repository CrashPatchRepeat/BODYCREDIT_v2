#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CMoveStrategy.h"

enum class ESkillCoolDown : uint8;
enum class ECombatState : uint8;
class ACNox_EBase;

struct FEnemySkill
{
private:
	float SkillWeight;
	ESkillCoolDown Skill;

public:
	FEnemySkill(ESkillCoolDown InSkill, float InWeight = 0.f): SkillWeight(InWeight), Skill(InSkill)
	{
	}

	ESkillCoolDown GetSkill() const { return Skill; }
	float GetWeight() const { return SkillWeight; }
};

/**
 * 
 */
class CConditionalMoveStrategy_MEMORY : public ICMoveStrategy
{
public:
	virtual void Move(ACNox_EBase* Owner, float DeltaTime) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;

private:
	bool bIsMove = false;
	float AcceptanceThreshold = 800.f;
	const float RangeAttackRange = 800.f;

	TArray<FEnemySkill> Skills;

	void CovertToCombatState(ACNox_EBase* Owner);
	void TryCombatStateTransition(ACNox_EBase* Owner);
	bool TrySpecialSkillTransition(ACNox_EBase* Owner, ECombatState& OutChosenSkill);
	bool TryDefaultAttackTransition(ACNox_EBase* Owner);


	bool ChooseRandomSkill(ACNox_EBase* Owner, ECombatState& OutChooseSkill);
	int32 CalculateTotalWeight(const TArray<int32>& SkillIndices) const;
	ECombatState ConvertSkillToCombatState(ESkillCoolDown Skill) const;
};
