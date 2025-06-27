#pragma once

#include "CoreMinimal.h"
#include "Characters/CNoxAnimInstance.h"
#include "Components/Runner/CWeaponComponent.h"
#include "CNoxAnimInstance_Runner.generated.h"

class UCWeaponComponent;

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

	UPROPERTY(blueprintreadonly, Category = "Movement")
	bool bIsCrouching = false;
	
	UFUNCTION(BlueprintPure, Category = "Movement", meta=(BlueprintThreadSafe))
	bool IsSprinting();

	UPROPERTY(blueprintreadonly, Category = "Movement")
	FRotator PrevRotation;

	UPROPERTY(blueprintreadonly, Category = "Movement")
	float Direction = 0.0f;

	UPROPERTY(blueprintreadonly, Category = "Movement")
	bool bFalling = false;

	UPROPERTY(blueprintreadonly, Category = "Movement")
	bool bFiring = false;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapons")
	EWeaponType WeaponType = EWeaponType::Max;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Pitch;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapons")
	bool bInAim;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapons")
	bool bUseIK;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapons")
	FVector LeftHandLocation;

private:
	UPROPERTY()
	UCWeaponComponent* Weapon;
	
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);
};
