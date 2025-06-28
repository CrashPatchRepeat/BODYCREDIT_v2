#pragma once

#include "CoreMinimal.h"
#include "Characters/CNox.h"
#include "CNox_Runner.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UCMovementComponent;
class UCWeaponComponent;
class UCMarketComponent;
class UCInventoryComponent;

class UInputMappingContext;

UCLASS()
class BODYCREDIT_V2_API ACNox_Runner : public ACNox
{
	GENERATED_BODY()

public:
	ACNox_Runner();

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	FORCEINLINE USkeletalMeshComponent* GetFPSArms() const { return FPSArms; }

#pragma region Flash Bang
	UFUNCTION(BlueprintNativeEvent)
	void ReactFlashBang(FVector InLocation);
	virtual void ReactFlashBang_Implementation(FVector InLocation) {};
#pragma endregion

protected:
	virtual void BeginPlay() override;

private:
#pragma region Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FVector2D PitchRange = FVector2D(-40, +40);

	void InitCameraAndSpringArm();
#pragma endregion

#pragma region CharacterSet
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSet", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Hair;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSet", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* UpperBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSet", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Outer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSet", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Arms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSet", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* LowerBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSet", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Foot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSet", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ChestRig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSet", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Backpack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSet", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FPSArms;
	
	void InitCharacterMeshes();
	void InitCharacterMovement();
#pragma endregion
	
#pragma region MovementComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomComponents", meta = (AllowPrivateAccess = "true"))
	UCMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomComponents", meta = (AllowPrivateAccess = "true"))
	UCWeaponComponent* WeaponComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomComponents", meta = (AllowPrivateAccess = "true"))
	UCMarketComponent* MarketComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomComponents", meta = (AllowPrivateAccess = "true"))
	UCInventoryComponent* InventoryComponent;
	
	void InitCustomComponents();
#pragma endregion

#pragma region EnhancedInput
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnhancedInput", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* IMC_Runner;

	void InitMappingContexts();
#pragma endregion
};
