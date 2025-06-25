// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CEStateStrategy.h"

class CRandomMoveStrategy_MEDIC;
class ACNox_EBase;
class UCNox_FSMComp;

/**
 * 
 */
class CIdleState_MEDIC : public ICEStateStrategy
{
public:
	CIdleState_MEDIC(TUniquePtr<CRandomMoveStrategy_MEDIC> InMoveStrategy);
	virtual void Execute(ACNox_EBase* Owner, UCNox_FSMComp* FSMComp) override;
	
private:
	TUniquePtr<CRandomMoveStrategy_MEDIC> MoveStrategy;	
};
