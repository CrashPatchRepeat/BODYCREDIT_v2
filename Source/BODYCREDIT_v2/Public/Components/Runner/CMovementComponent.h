#pragma once

#include "CoreMinimal.h"
#include "Components/CBaseComponent.h"
#include "CMovementComponent.generated.h"

class UInputAction;
class UEnhancedInputComponent;

UENUM(BlueprintType)
enum class EMoveDirection : uint8
{
	Forward = 0, // 정면(W)
	Backward,    // 뒤(S)
	Left,        // 왼쪽(A)
	Right,       // 오른쪽(D)
	Max
};

UENUM(BlueprintType)
enum class ESpeedType : uint8
{
	Crouch = 0, Walk, Run, Sprint, Max
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
	}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BODYCREDIT_V2_API UCMovementComponent : public UCBaseComponent
{
	GENERATED_BODY()

public:
	UCMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupInputBindings(UEnhancedInputComponent* InEnhancedInputComponent);

protected:
	virtual void BeginPlay() override;

private:
	void InitInputAction();

#pragma region Speed
	UPROPERTY(EditAnywhere, Category = "Speed")
	FSpeedSet SpeedSets[(int32)ESpeedType::Max];

	// 속도 보간
	float TargetSpeed = 0.f;    	// 목표 속도
	float SpeedInterpRate = 10.f;	// 보간 계수
	
	void InitSpeedSets();

	// 속도 설정 함수
	void SetSpeed(ESpeedType InType, EMoveDirection InDirection);
#pragma endregion

#pragma region Movement
	// IA_Movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnhancedInput", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Movement;

	// 이동 가능 여부
	bool bCanMove = true;

	// W키가 눌렸는지 여부
	bool bIsWKeyDown = false;

	// 현재 속도 타입
	ESpeedType CurrentSpeedType = ESpeedType::Walk;

	// 현재 입력 방향
	EMoveDirection CurrentMoveDirection = EMoveDirection::Forward;

	// 이동 함수
	void OnMovement(const struct FInputActionValue& InVal);
	void OffMovement(const struct FInputActionValue& InVal);

	// 현재 방향 계산 함수
	EMoveDirection GetMoveDirection(const FVector2D& MoveInput) const;

	// 현재 속도 계산 함수
	float GetCurrentSpeed() const;
#pragma endregion

#pragma region Look
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnhancedInput", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float HorizontalLook = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float VerticalLook = 50.0f;

	bool bFixedCamera = false;

	void OnLook(const struct FInputActionValue& InVal);
#pragma endregion

#pragma region Sprint
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnhancedInput", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Sprint;

	// Sprint 키가 눌렸는지 여부
	bool bIsSprintKeyDown = false;

	void OnSprint(const struct FInputActionValue& InVal);
	void OffSprint(const struct FInputActionValue& InVal);
#pragma endregion

#pragma region Jump
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnhancedInput", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Jump;

	void OnJump(const struct FInputActionValue& InVal);
#pragma endregion

#pragma region Crouch
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnhancedInput", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Crouch;

	bool bIsCrouching = false;

	// 커스텀 크라우치 보간 변수
	float StandCapsuleHalfHeight = 0.f;
	float CrouchCapsuleHalfHeight = 0.f;
	float CapsuleInterpSpeed = 12.f;
	float TargetCapsuleHalfHeight = 0.f;

	void OnCrouch(const struct FInputActionValue& InVal);
#pragma endregion

};
