#pragma once

#include "CoreMinimal.h"
#include "Characters/CNoxAnimInstance.h"
#include "Components/Runner/CWeaponComponent.h"
#include "CNoxAnimInstance_RunnerArms.generated.h"

class UCWeaponComponent;

UCLASS()
class BODYCREDIT_V2_API UCNoxAnimInstance_RunnerArms : public UCNoxAnimInstance
{
	GENERATED_BODY()

public:
	void NativeBeginPlay() override;
	
	void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapons")
	EWeaponType WeaponType = EWeaponType::Max;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapons")
	FTransform LeftHandTransform;

private:
	UPROPERTY()
	UCWeaponComponent* Weapon;
	
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);
};
