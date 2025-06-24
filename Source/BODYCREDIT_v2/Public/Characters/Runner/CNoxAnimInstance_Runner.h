#pragma once

#include "CoreMinimal.h"
#include "Characters/CNoxAnimInstance.h"
#include "CNoxAnimInstance_Runner.generated.h"

UCLASS()
class BODYCREDIT_V2_API UCNoxAnimInstance_Runner : public UCNoxAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(blueprintreadonly, Category = "Movement")
	float Speed = 0.0f;

	UFUNCTION(BlueprintPure, Category = "Movement", meta=(BlueprintThreadSafe))
	bool IsRunning();

	UFUNCTION(BlueprintPure, Category = "Movement", meta=(BlueprintThreadSafe))
	bool IsSprinting();

	UPROPERTY(blueprintreadonly, Category = "Movement")
	FRotator PrevRotation;

	UPROPERTY(blueprintreadonly, Category = "Movement")
	float Direction = 0.0f;

	UPROPERTY(blueprintreadonly, Category = "Movement")
	bool bIsCrouching = false;
};
