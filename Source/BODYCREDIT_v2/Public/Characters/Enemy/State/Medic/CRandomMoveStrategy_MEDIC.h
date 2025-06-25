// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CMoveStrategy.h"

/**
 * 
 */
class CRandomMoveStrategy_MEDIC : public ICMoveStrategy
{
public:
	virtual void Move(ACNox_EBase* Owner, float DeltaTime) override;

private:
	float RandomRadius = 1000;
	float AcceptanceThreshold = 10.f;
	bool bMoving = false;
	FVector RanLocation=FVector::ZeroVector;

	FVector LastPosition = FVector::ZeroVector; // 마지막 위치
	float StuckTime = 0.f; // 한 자리에 머무른 시간

	void HearingMove(ACNox_EBase* Owner);
	void RandomMove(ACNox_EBase* Owner);
	FVector GetRandomLocation(const ACNox_EBase* Owner) const;
};
