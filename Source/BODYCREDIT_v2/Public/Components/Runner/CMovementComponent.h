#pragma once

#include "CoreMinimal.h"
#include "Components/CBaseComponent.h"
#include "CMovementComponent.generated.h"

class UInputAction;
class UEnhancedInputComponent;

UENUM(BlueprintType)
enum class EMoveDirection : uint8
{
	Forward = 0,
	Backward,
	Left,
	Right,
	Max
};

UENUM(BlueprintType)
enum class ESpeedType : uint8
{
	Walk = 0,
	Sprint,
	Crouch,
	Max
};

USTRUCT(BlueprintType)
struct FSpeedSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Speed;

	FSpeedSet()
	{
		Speed.SetNum((int32)EMoveDirection::Max);
		Speed[(int32)EMoveDirection::Forward] = 500.f;
		Speed[(int32)EMoveDirection::Backward] = 300.f;
		Speed[(int32)EMoveDirection::Left] = 350.f;
		Speed[(int32)EMoveDirection::Right] = 350.f;
	}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BODYCREDIT_V2_API UCMovementComponent : public UCBaseComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE float GetWalkSpeed() { return SpeedSets[(int32)ESpeedType::Walk].Speed[(int32)EMoveDirection::Forward]; }
	FORCEINLINE float GetRunSpeed() { return SpeedSets[(int32)ESpeedType::Walk].Speed[(int32)EMoveDirection::Forward]; }
	FORCEINLINE float GetSprintSpeed() { return SpeedSets[(int32)ESpeedType::Sprint].Speed[(int32)EMoveDirection::Forward]; }
	
	
	UCMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupInputBindings(UEnhancedInputComponent* InEnhancedInputComponent);
	
	void SetSpeed(ESpeedType InType, EMoveDirection InDirection);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnhancedInput", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnhancedInput", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Look;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnhancedInput", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Sprint;
	
	UPROPERTY(EditAnywhere, Category = "Speed")
	FSpeedSet SpeedSets[(int32)ESpeedType::Max];

	ESpeedType CurrentSpeedType = ESpeedType::Walk;
	EMoveDirection CurrentMoveDirection = EMoveDirection::Forward;
	bool bIsSprintKeyDown = false;
	bool bIsCrouching = false;
	bool bIsWKeyDown = false;

	void InitInputAction();

	void OnMovement(const struct FInputActionValue& InVal);
	void OffMovement(const struct FInputActionValue& InVal);
	
	void OnLook(const struct FInputActionValue& InVal);
	
	void OnSprint(const struct FInputActionValue& InVal);
	void OffSprint(const struct FInputActionValue& InVal);

	EMoveDirection GetMoveDirection(const FVector2D& MoveInput) const;
	float GetCurrentSpeed() const;
};
