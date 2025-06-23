#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/State/CEnemyState.h"
#include "Components/CBaseComponent.h"
#include "Global.h"
#include "CNox_FSMComp.generated.h"

class ACNox_EBase;
class ICEnemyStateStrategy;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BODYCREDIT_V2_API UCNox_FSMComp : public UCBaseComponent
{
	GENERATED_BODY()

public:
#pragma region 생성자_및_초기화
	UCNox_FSMComp();
	void InitializeFSM(ACNox_EBase* InOwner);
#pragma endregion

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
	void UpdateSkillCoolDowns(ESkillCoolDown Skill, float DeltaTime)
	{
		SkillCoolDowns[GetSkillName(Skill)] += DeltaTime;
		CLog::Log(FString::Printf(TEXT("SkillCoolDowns[%s] = %f"), *GetSkillName(Skill).ToString(), SkillCoolDowns[GetSkillName(Skill)]));
	}
	bool IsSkillReady(ESkillCoolDown Skill) const
	{
		return SkillCoolDowns[GetSkillName(Skill)] >= SkillMaxCoolDowns[GetSkillName(Skill)];
	}
	void UsingSkill(ESkillCoolDown Skill)
	{
		SkillCoolDowns[GetSkillName(Skill)] = 0.f;
	}
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
	TMap<EEnemyState, TSharedPtr<ICEnemyStateStrategy>> EnemyStrategies;
	TMap<EEnemyState, TSharedPtr<ICEnemyStateStrategy>> CreateStrategies(EEnemyType Type);
#pragma endregion

#pragma region 스킬_쿨타임_데이터
	TMap<FName, float> SkillCoolDowns;
	TMap<FName, float> SkillMaxCoolDowns;
	void InitSkillCoolDowns(EEnemyType Type);
#pragma endregion
};
