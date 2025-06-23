#include "Components/Runner/CMovementComponent.h"
#include "Global.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Characters/CNox.h"
#include "GameFramework/CharacterMovementComponent.h"

UCMovementComponent::UCMovementComponent()
{
	InitInputAction();
}

void UCMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCMovementComponent::SetupInputBindings(UEnhancedInputComponent* InEnhancedInputComponent)
{
	CheckNull(InEnhancedInputComponent);

	// Movement
	CheckNull(IA_Movement);
	InEnhancedInputComponent->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &UCMovementComponent::OnMovement);
	InEnhancedInputComponent->BindAction(IA_Movement, ETriggerEvent::Completed, this, &UCMovementComponent::OffMovement);

	// Look
	CheckNull(IA_Look);
	InEnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UCMovementComponent::OnLook);

	// Sprint
	CheckNull(IA_Sprint);
	InEnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &UCMovementComponent::OnSprint);
	InEnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &UCMovementComponent::OffSprint);
}

void UCMovementComponent::SetSpeed(ESpeedType InType, EMoveDirection InDirection)
{
	if (!OwnerCharacter) return;
	if (!OwnerCharacter->GetCharacterMovement()) return;

	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = SpeedSets[(int32)InType].Speed[(int32)InDirection];
}

void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCMovementComponent::InitInputAction()
{
	// IA_Movement
	CHelpers::GetAsset<UInputAction>(&IA_Movement, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Movement.IA_Movement'"));

	// IA_Look
	CHelpers::GetAsset<UInputAction>(&IA_Look, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Look.IA_Look'"));

	// IA_Sprint
	CHelpers::GetAsset<UInputAction>(&IA_Sprint, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Sprint.IA_Sprint'"));
}

void UCMovementComponent::OnMovement(const struct FInputActionValue& InVal)
{
	FVector2D MoveInput = InVal.Get<FVector2D>();
	CurrentMoveDirection = GetMoveDirection(MoveInput);
	bIsWKeyDown = (MoveInput.Y > 0 && MoveInput.X == 0);

	if (bIsCrouching)
		CurrentSpeedType = ESpeedType::Crouch;
	else if (bIsSprintKeyDown && CurrentMoveDirection == EMoveDirection::Forward)
		CurrentSpeedType = ESpeedType::Sprint;
	else
		CurrentSpeedType = ESpeedType::Walk;

	SetSpeed(CurrentSpeedType, CurrentMoveDirection);

	// 실제 캐릭터 이동 처리
	if (OwnerCharacter)
	{
		FRotator ControlRot = OwnerCharacter->GetController() ? OwnerCharacter->GetController()->GetControlRotation() : FRotator::ZeroRotator;
		// Pitch, Roll 제거 (Yaw만 사용)
		FRotator YawRot(0, ControlRot.Yaw, 0);
		FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X); // 컨트롤러 기준 정면(X)
		FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);   // 컨트롤러 기준 오른쪽(Y)
		OwnerCharacter->AddMovementInput(Forward, MoveInput.X); // X축이 정면
		OwnerCharacter->AddMovementInput(Right, MoveInput.Y);   // Y축이 오른쪽
	}
}

void UCMovementComponent::OffMovement(const struct FInputActionValue& InVal)
{
}

void UCMovementComponent::OnLook(const struct FInputActionValue& InVal)
{
}

void UCMovementComponent::OnSprint(const struct FInputActionValue& InVal)
{
	bIsSprintKeyDown = true;
	if (bIsWKeyDown && !bIsCrouching)
		CurrentSpeedType = ESpeedType::Sprint;
}

void UCMovementComponent::OffSprint(const struct FInputActionValue& InVal)
{
	bIsSprintKeyDown = false;
	if (bIsCrouching)
		CurrentSpeedType = ESpeedType::Crouch;
	else
		CurrentSpeedType = ESpeedType::Walk;
}

EMoveDirection UCMovementComponent::GetMoveDirection(const FVector2D& MoveInput) const
{
	if (MoveInput.Y > 0)
		return EMoveDirection::Forward;
	if (MoveInput.Y < 0)
		return EMoveDirection::Backward;
	if (MoveInput.X < 0)
		return EMoveDirection::Left;
	if (MoveInput.X > 0)
		return EMoveDirection::Right;
	return EMoveDirection::Forward;
}

float UCMovementComponent::GetCurrentSpeed() const
{
	return SpeedSets[(int32)CurrentSpeedType].Speed[(int32)CurrentMoveDirection];
}
