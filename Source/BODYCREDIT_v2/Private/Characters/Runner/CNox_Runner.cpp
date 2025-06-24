#include "Characters/Runner/CNox_Runner.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Runner/CMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Games/CNoxController.h"

ACNox_Runner::ACNox_Runner()
{
	InitCameraAndSpringArm();
	InitCharacterMeshes();
	InitCharacterMovement();
	InitCustomComponents();
	InitMappingContexts();
}

void ACNox_Runner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACNox_Runner::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// MappingContext
	if (ACNoxController* controller = Cast<ACNoxController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(controller->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Runner, 0);
		}
	}
}

void ACNox_Runner::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	CheckNull(input);

	// MovementComponent
	MovementComponent->SetupInputBindings(input);
}

void ACNox_Runner::BeginPlay()
{
	Super::BeginPlay();
}

void ACNox_Runner::InitCameraAndSpringArm()
{
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArmComponent, TEXT("SpringArmComponent"), RootComponent);
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->TargetArmLength = 200;
	SpringArmComponent->bUsePawnControlRotation = true;

	CHelpers::CreateComponent<UCameraComponent>(this, &CameraComponent, TEXT("CameraComponent"), SpringArmComponent);
	CameraComponent->SetRelativeLocation(FVector(-80, 60, 100));
	CameraComponent->bUsePawnControlRotation = false;
}

void ACNox_Runner::InitCharacterMeshes()
{
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
}

void ACNox_Runner::InitCharacterMovement()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void ACNox_Runner::InitCustomComponents()
{
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &MovementComponent, TEXT("CMovementComponent"));
}

void ACNox_Runner::InitMappingContexts()
{
	CHelpers::GetAsset<UInputMappingContext>(&IMC_Runner,TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IMC_Runner.IMC_Runner'"));
}
