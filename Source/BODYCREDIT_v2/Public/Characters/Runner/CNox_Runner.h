#pragma once

#include "CoreMinimal.h"
#include "Characters/CNox.h"
#include "CNox_Runner.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UCMovementComponent;

class UInputMappingContext;

UCLASS()
class BODYCREDIT_V2_API ACNox_Runner : public ACNox
{
	GENERATED_BODY()

public:
	ACNox_Runner();

	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region Flash Bang
	UFUNCTION(BlueprintNativeEvent)
	void ReactFlashBang(FVector InLocation);
	virtual void ReactFlashBang_Implementation(FVector InLocation) {};
#pragma endregion

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomComponents", meta = (AllowPrivateAccess = "true"))
	UCMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnhancedInput", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* IMC_Runner;

	void InitCameraAndSpringArm();
	void InitCharacterMeshes();
	void InitCharacterMovement();
	void InitCustomComponents();
	void InitMappingContexts();
};
