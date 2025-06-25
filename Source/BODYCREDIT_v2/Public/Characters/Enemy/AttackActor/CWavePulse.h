#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWavePulse.generated.h"

UCLASS()
class BODYCREDIT_V2_API ACWavePulse : public AActor
{
	GENERATED_BODY()
	
public:
#pragma region Wave Pulse
	void StartWave();
#pragma endregion
	
private:	
	ACWavePulse();
	virtual void BeginPlay() override;

#pragma region Wave Pulse
	UPROPERTY()
	class UTimelineComponent* WaveTimeline;
	UPROPERTY(EditAnywhere)
	UCurveFloat* ScaleCurve;
	UPROPERTY(EditAnywhere)
	float MaxScale = 50.0f;
	UPROPERTY(EditAnywhere)
	FRotator MaxRotation = FRotator(45.f, 90.f, 45.f); // Pitch, Yaw, Roll 모두 포함
	UPROPERTY()
	TSet<AActor*> DamagedActors;
	
	UFUNCTION()
	void HandleWaveProgress(float Value);
	UFUNCTION()
	void OnWaveEnd();
#pragma endregion

#pragma region 컴포넌트
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* SphereComp;
#pragma endregion
	
	UPROPERTY(EditDefaultsOnly)
	class ACNox_EBase* OwnerAI;

};
