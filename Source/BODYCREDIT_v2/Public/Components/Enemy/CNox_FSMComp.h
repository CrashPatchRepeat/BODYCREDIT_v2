#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/State/CEnemyState.h"
#include "Components/CBaseComponent.h"
#include "CNox_FSMComp.generated.h"

class ICEStateStrategy;
class ACNox_EBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BODYCREDIT_V2_API UCNox_FSMComp : public UCBaseComponent
{
	GENERATED_BODY()

public:
#pragma region 상태_관리
	void UpdateState();
	void ResetVal(EEnemyType Type);
	EEnemyState GetEnemyState() const { return CurrentEnemyState; }
	ECombatState GetCombatState() const { return CurrentCombatState; }
	void SetEnemyState(EEnemyState NewState);
	void SetCombatState(ECombatState NewCombatState);
#pragma endregion

#pragma region 스킬_쿨타임_관리
	FName GetSkillName(ESkillCoolDown SkillType) const;
	void UpdateSkillCoolDowns(ESkillCoolDown Skill, float DeltaTime);
	bool IsSkillReady(ESkillCoolDown Skill) const;
	void UsingSkill(ESkillCoolDown Skill);	
#pragma endregion

protected:
#pragma region UE_오버라이드
	virtual void BeginPlay() override;
#pragma endregion

private:
#pragma region 내부_상태
	UPROPERTY()
	ACNox_EBase* OwnerEnemy = nullptr;
	EEnemyState CurrentEnemyState = EEnemyState::IDLE;
	ECombatState CurrentCombatState = ECombatState::Default;
	TMap<EEnemyState, TSharedPtr<ICEStateStrategy>> EnemyStrategies;
	TMap<EEnemyState, TSharedPtr<ICEStateStrategy>> CreateStrategies(EEnemyType Type);
#pragma endregion

#pragma region 스킬_쿨타임_데이터
	TMap<FName, float> SkillCoolDowns;
	TMap<FName, float> SkillMaxCoolDowns;
	void InitSkillCoolDowns(EEnemyType Type);
#pragma endregion
};
