#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Interface/CMoveStrategy.h"

/**
 * 
 */
class BODYCREDIT_V2_API CRandomMoveStrategy_ZERO: public ICMoveStrategy
{
public:
	virtual void Move(ACNox_EBase* Owner, float DeltaTime) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;
	
private:
	TArray<float> Distances; // 각 포인트 거리
	int32 CurrentIndex = 0;

	float AcceptanceRadius = 10.f;
	float AcceptanceThreshold = 50.f;
	bool bMoving = false;

	FVector LastPosition = FVector::ZeroVector; // 마지막 위치
	float StuckTime = 0.f; // 한 자리에 머무른 시간

	float RandomRadius = 400;
	FVector RanLocation = FVector::ZeroVector;

	void RandomMove(ACNox_EBase* Owner);
	void HearingMove(ACNox_EBase* Owner);
	FVector GetRandomLocation(const ACNox_EBase* Owner) const;
};