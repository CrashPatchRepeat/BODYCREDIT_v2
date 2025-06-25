// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CMoveStrategy.h"

class ACNox_EBase;

/**
 * 
 */
class BODYCREDIT_V2_API CMemoryMoveStrategy : public ICMoveStrategy
{
public:
	virtual void Move(ACNox_EBase* Owner, float DeltaTime) override;
	
private:
	enum EMemoryMoveState : uint8
	{
		None,
		MoveToStairState,
		MoveToVentState,
		MoveToMemoryState,
	};

	EMemoryMoveState CurMoveState = EMemoryMoveState::None;
	FVector MovementTargetLoc = FVector::ZeroVector;
	FVector NextTargetAfterTransfer = FVector::ZeroVector;

	bool IsOtherFloor(ACNox_EBase* Owner);
	void EvaluateTransferPath(ACNox_EBase* Owner);
	void MoveToMemory(ACNox_EBase* Owner);
	void MoveToStair(ACNox_EBase* Owner);
	void MoveToVent(ACNox_EBase* Owner);
	void EndStrategy(ACNox_EBase* Owner);
	bool MoveToLoc(ACNox_EBase* Owner, const FVector TargetLoc);
	//===============
	float AcceptanceThreshold = 10.f;
	bool bMoving = false;
};
