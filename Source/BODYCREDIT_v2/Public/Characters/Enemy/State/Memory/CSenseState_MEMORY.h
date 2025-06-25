// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"

class CConditionalMoveStrategy_MEMORY;
class ACNox_EBase;
class UCNox_FSMComp;

/**
 * 
 */
class BODYCREDIT_V2_API CSenseState_MEMORY : public ICEStateStrategy
{
public:
	CSenseState_MEMORY(TUniquePtr<CConditionalMoveStrategy_MEMORY> InMoveStrategy);
	virtual void Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;

private:
	TUniquePtr<CConditionalMoveStrategy_MEMORY> MoveStrategy;
};
