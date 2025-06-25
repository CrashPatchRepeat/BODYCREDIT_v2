#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBeam.generated.h"

class ACNox_Memory;
class UNiagaraComponent;

UCLASS()
class BODYCREDIT_V2_API ACBeam : public AActor
{
	GENERATED_BODY()
	
public:	
	void SetBeamActive(bool bInActive, AActor* InTarget);

private:
	ACBeam();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
#pragma region 컴포넌트 생성 및 초기화
	void CreateComponents();
	void SetupVFXAssets();
	void DeactivateAllVFX();
#pragma endregion

#pragma region 컴포넌트
	UPROPERTY(VisibleAnywhere)
	USceneComponent* rootScene;
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* LaserBeamVFX;
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* FireBallVFX;
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* HitVFX;
#pragma endregion

#pragma region 빔 활성화/비활성화
	bool bApplyDamage = false;
	float DamageTimer = 0.0f;
	float CurPitch = 0.0f;
	FTimerHandle ActiveTimer;
	bool AttackStart = false;
	float ActiveDelay = 0.5f;
	
	void InitializeBeam(AActor* InTarget);
	void DeactivateBeam();
#pragma endregion

#pragma region 빔 업데이트
	float DamageInterval = 0.3f;
	float CurAttackDelay = 0.0f;
	float AttackDelay = 1.0f;
	
	void UpdateDamageTimer(float DeltaTime);
	void UpdateBeamPosition(float DeltaTime);
	void UpdateAttackDelay(float DeltaTime);
#pragma endregion

#pragma region 빔 계산
	float AttackRange = 1000.0f;
	
	FVector CalculateBeamEnd(float DeltaTime);
	bool PerformLineTrace(const FVector& Start, const FVector& End, FHitResult& OutHitResult);
#pragma endregion

#pragma region 빔 충돌
	void HandleBeamHit(const FHitResult& HitResult);
	void HandleBeamMiss(const FVector& EndLocation);
#pragma endregion
	
	UPROPERTY()
	ACNox_Memory* OwnerAI;
	UPROPERTY()
	AActor* TargetActor;
};
