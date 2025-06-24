#include "Components/Runner/CMovementComponent.h"
#include "Global.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Characters/CNox.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

UCMovementComponent::UCMovementComponent()
{
	InitSpeedSets();
	InitInputAction();
}

void UCMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckNull(OwnerCharacter);

	// --- 캡슐 반경 보간 ---
	if (UCapsuleComponent* Capsule = CHelpers::GetComponent<UCapsuleComponent>(OwnerCharacter))
	{
		float CurrentHalf = Capsule->GetUnscaledCapsuleHalfHeight();
		float NewHalf = FMath::FInterpTo(CurrentHalf, TargetCapsuleHalfHeight, DeltaTime, CapsuleInterpSpeed);
		
		Capsule->SetCapsuleHalfHeight(NewHalf, true);
	}
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

	// Jump
	CheckNull(IA_Jump);
	InEnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &UCMovementComponent::OnJump);

	// Crouch
	CheckNull(IA_Crouch);
	InEnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &UCMovementComponent::OnCrouch);
}

void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	CheckNull(OwnerCharacter);

	// 캡슐 높이 초기화
	if (UCapsuleComponent* Capsule = CHelpers::GetComponent<UCapsuleComponent>(OwnerCharacter))
	{
		StandCapsuleHalfHeight = Capsule->GetUnscaledCapsuleHalfHeight();
		CrouchCapsuleHalfHeight = OwnerCharacter->GetCharacterMovement()->GetCrouchedHalfHeight();
		TargetCapsuleHalfHeight = StandCapsuleHalfHeight;
	}
}

void UCMovementComponent::InitInputAction()
{
	// IA_Movement
	CHelpers::GetAsset<UInputAction>(&IA_Movement, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Movement.IA_Movement'"));

	// IA_Look
	CHelpers::GetAsset<UInputAction>(&IA_Look, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Look.IA_Look'"));

	// IA_Sprint
	CHelpers::GetAsset<UInputAction>(&IA_Sprint, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Sprint.IA_Sprint'"));

	// IA_Jump
	CHelpers::GetAsset<UInputAction>(&IA_Jump, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Jump.IA_Jump'"));

	// IA_Crouch
	CHelpers::GetAsset<UInputAction>(&IA_Crouch, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Crouch.IA_Crouch'"));
}

#pragma region Speed
void UCMovementComponent::InitSpeedSets()
{
	// Crouch: {225, 180, 200, 200} (정면, 뒤, 왼쪽, 오른쪽)
	SpeedSets[(int32)ESpeedType::Crouch].Speed[(int32)EMoveDirection::Forward] = 225.f;
	SpeedSets[(int32)ESpeedType::Crouch].Speed[(int32)EMoveDirection::Backward] = 180.f;
	SpeedSets[(int32)ESpeedType::Crouch].Speed[(int32)EMoveDirection::Left] = 200.f;
	SpeedSets[(int32)ESpeedType::Crouch].Speed[(int32)EMoveDirection::Right] = 200.f;

	// Walk: {200, 150, 180, 180}
	SpeedSets[(int32)ESpeedType::Walk].Speed[(int32)EMoveDirection::Forward] = 200.f;
	SpeedSets[(int32)ESpeedType::Walk].Speed[(int32)EMoveDirection::Backward] = 150.f;
	SpeedSets[(int32)ESpeedType::Walk].Speed[(int32)EMoveDirection::Left] = 180.f;
	SpeedSets[(int32)ESpeedType::Walk].Speed[(int32)EMoveDirection::Right] = 180.f;

	// Run: {500, 300, 350, 350}
	SpeedSets[(int32)ESpeedType::Run].Speed[(int32)EMoveDirection::Forward] = 500.f;
	SpeedSets[(int32)ESpeedType::Run].Speed[(int32)EMoveDirection::Backward] = 300.f;
	SpeedSets[(int32)ESpeedType::Run].Speed[(int32)EMoveDirection::Left] = 350.f;
	SpeedSets[(int32)ESpeedType::Run].Speed[(int32)EMoveDirection::Right] = 350.f;

	// Sprint: {700, 700, 700, 700}
	SpeedSets[(int32)ESpeedType::Sprint].Speed[(int32)EMoveDirection::Forward] = 700.f;
	SpeedSets[(int32)ESpeedType::Sprint].Speed[(int32)EMoveDirection::Backward] = 700.f;
	SpeedSets[(int32)ESpeedType::Sprint].Speed[(int32)EMoveDirection::Left] = 700.f;
	SpeedSets[(int32)ESpeedType::Sprint].Speed[(int32)EMoveDirection::Right] = 700.f;
}

void UCMovementComponent::SetSpeed(ESpeedType InType, EMoveDirection InDirection)
{
	CheckNull(OwnerCharacter);

	UCharacterMovementComponent* movement = OwnerCharacter->GetCharacterMovement();
	CheckNull(OwnerCharacter->GetCharacterMovement());

	if (InType == ESpeedType::Crouch) movement->MaxWalkSpeedCrouched = SpeedSets[(int32)InType].Speed[(int32)InDirection];
	else movement->MaxWalkSpeed = SpeedSets[(int32)InType].Speed[(int32)InDirection];
}
#pragma endregion

#pragma region Movement
void UCMovementComponent::OnMovement(const struct FInputActionValue& InVal)
{
	CheckNull(OwnerCharacter);
	CheckFalse(bCanMove);

	FVector2D MoveInput = InVal.Get<FVector2D>();
	if (MoveInput.SizeSquared() > 1.0f) MoveInput.Normalize();

	// 방향 판정
	CurrentMoveDirection = GetMoveDirection(MoveInput);
	bIsWKeyDown = (MoveInput.X > 0);

	// 상태 결정
	if (bIsCrouching) CurrentSpeedType = ESpeedType::Crouch;
	else if (bIsSprintKeyDown && CurrentMoveDirection == EMoveDirection::Forward) CurrentSpeedType = ESpeedType::Sprint;
	else if (CurrentSpeedType == ESpeedType::Sprint && CurrentMoveDirection != EMoveDirection::Forward) CurrentSpeedType = ESpeedType::Run;
	else CurrentSpeedType = ESpeedType::Run;

	// 속도 적용
	SetSpeed(CurrentSpeedType, CurrentMoveDirection);

	// 실제 이동
	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector forward = FQuat(rotator).GetForwardVector();
	FVector right = FQuat(rotator).GetRightVector();

	OwnerCharacter->AddMovementInput(forward, MoveInput.X); // X축이 정면(W/S)
	OwnerCharacter->AddMovementInput(right, MoveInput.Y);   // Y축이 오른쪽(D/A)
}

void UCMovementComponent::OffMovement(const struct FInputActionValue& InVal)
{
}

EMoveDirection UCMovementComponent::GetMoveDirection(const FVector2D& MoveInput) const
{
	if (MoveInput.X > 0) return EMoveDirection::Forward;
	if (MoveInput.X < 0) return EMoveDirection::Backward;
	if (MoveInput.Y > 0) return EMoveDirection::Right;
	if (MoveInput.Y < 0) return EMoveDirection::Left;
	return EMoveDirection::Forward;
}

float UCMovementComponent::GetCurrentSpeed() const
{
	return SpeedSets[(int32)CurrentSpeedType].Speed[(int32)CurrentMoveDirection];
}
#pragma endregion

#pragma region Look
void UCMovementComponent::OnLook(const struct FInputActionValue& InVal)
{
	CheckNull(OwnerCharacter);
	CheckTrue(bFixedCamera);

	FVector2D LookInput = InVal.Get<FVector2D>();

	OwnerCharacter->AddControllerYawInput(LookInput.X * HorizontalLook * GetWorld()->GetDeltaSeconds());
	OwnerCharacter->AddControllerPitchInput(LookInput.Y * VerticalLook * GetWorld()->GetDeltaSeconds());
}
#pragma endregion

#pragma region Sprint
void UCMovementComponent::OnSprint(const struct FInputActionValue& InVal)
{
	bIsSprintKeyDown = true;
}

void UCMovementComponent::OffSprint(const struct FInputActionValue& InVal)
{
	bIsSprintKeyDown = false;
}
#pragma endregion

#pragma region Jump
void UCMovementComponent::OnJump(const struct FInputActionValue& InVal)
{
	CheckNull(OwnerCharacter);

	OwnerCharacter->Jump();
}
#pragma endregion

#pragma region Crouch
void UCMovementComponent::OnCrouch(const struct FInputActionValue& InVal)
{
	CheckNull(OwnerCharacter);

	if (bIsCrouching)
	{
		OwnerCharacter->UnCrouch();
		bIsCrouching = false;
		TargetCapsuleHalfHeight = StandCapsuleHalfHeight;
	}
	else
	{
		OwnerCharacter->Crouch();
		bIsCrouching = true;
		TargetCapsuleHalfHeight = CrouchCapsuleHalfHeight;
	}
}
#pragma endregion
