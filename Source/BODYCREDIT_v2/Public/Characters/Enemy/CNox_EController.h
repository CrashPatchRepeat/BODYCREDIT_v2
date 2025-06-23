#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CNox_EController.generated.h"

class ACNox;
class ACNox_EBase;

struct FActorPerceptionUpdateInfo;
DECLARE_DELEGATE_OneParam(FDetectPlayer, ACNox*);
/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API ACNox_EController : public AAIController
{
	GENERATED_BODY()

public:

#pragma region CCTV BroadCasting
	FDetectPlayer OnDetectPlayer;
#pragma endregion

#pragma region Stop Perception (Using Die)
	void PerceptionDeactive();
#pragma endregion

#pragma region Get Target
	ACNox* GetTarget() const { return TargetPlayer; }
#pragma endregion
	
private:
#pragma region Common
	UPROPERTY()
	ACNox_EBase* EnemyBase;
	UPROPERTY()
	ACNox* TargetPlayer;
#pragma endregion

#pragma region Tick
	virtual void Tick(float DeltaSeconds) override;
#pragma endregion

#pragma region Init
	ACNox_EController();
	virtual void OnPossess(APawn* InPawn) override;
	void InitPerception();
	void SetupSightConfig();
	void SetupHearingConfig();
#pragma endregion

#pragma region Sensing
	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* Perception;
	UPROPERTY()
	class UAISenseConfig_Hearing* Hearing;
	UPROPERTY()
	class UAISenseConfig_Sight* Sight;

	// Sensing Delegate Function
	UFUNCTION()
	void OnAITargetPerceptionInfoUpdate(const FActorPerceptionUpdateInfo& UpdateInfo);
#pragma endregion

#pragma region Get Near Player
	ACNox* GetNearTargetPlayer();
#pragma endregion

#pragma region Target loss
	float CurExpiredTime = 0.f;
	void UpdateExpiredStimuli(float DeltaTime);
#pragma endregion
};
