// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CAttackStrategy.h"

class ACNox_EBase;
class UCNox_FSMComp;

/**
 * 
 */
class BODYCREDIT_V2_API CDefaultAttackStrategy_ZERO : public ICAttackStrategy
{
public:
	virtual void Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;

private:
	bool bFired = false;
	float AcceptanceThreshold = 30.f;
};
