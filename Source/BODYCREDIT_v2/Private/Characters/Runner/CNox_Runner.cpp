#include "Characters/Runner/CNox_Runner.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/CNoxAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Runner/CMovementComponent.h"
#include "Components/Runner/CWeaponComponent.h"
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

void ACNox_Runner::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// Main Mesh를 Leader로 설정
	USkeletalMeshComponent* leader = GetMesh();
	if (leader == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACNox_Runner::PostInitializeComponents - LeaderMesh is null."));
		return;
	}

	// LeaderMesh에 SetupAttachment된 모든 SkeletalMeshComponent 자식 컴포넌트에 대해 LeaderPose 설정
	for (USceneComponent* child : leader->GetAttachChildren())
	{
		// 원본 코드를 보존하며, SkeletalMeshComponent만 처리
		if (USkeletalMeshComponent* skel = Cast<USkeletalMeshComponent>(child))
		{
			skel->SetLeaderPoseComponent(leader);
		}
	}
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

	// WeaponComponent
	WeaponComponent->SetupInputBindings(input);
}

void ACNox_Runner::BeginPlay()
{
	Super::BeginPlay();

	GetController<APlayerController>()->PlayerCameraManager->ViewPitchMin = PitchRange.X;
	GetController<APlayerController>()->PlayerCameraManager->ViewPitchMax = PitchRange.Y;
}

#pragma region Camera
void ACNox_Runner::InitCameraAndSpringArm()
{
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArmComponent, TEXT("SpringArmComponent"), RootComponent);
	SpringArmComponent->SetRelativeLocation(FVector(0, 0, 60));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->TargetArmLength = 200;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bEnableCameraLag = true;

	CHelpers::CreateComponent<UCameraComponent>(this, &CameraComponent, TEXT("CameraComponent"), SpringArmComponent);
	// CameraComponent->SetRelativeLocation(FVector(-80, 60, 100));
	CameraComponent->bUsePawnControlRotation = false;
}
#pragma endregion

#pragma region CharacterSet
void ACNox_Runner::InitCharacterMeshes()
{
	USkeletalMesh* mesh;

	// Head
	CHelpers::GetAsset(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Runner/Meshes/Scifi_Character05/Scifi_Character05_Head.Scifi_Character05_Head'"));
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	// Hair
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Hair, TEXT("Hair"), GetMesh());
	CHelpers::GetAsset(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Runner/Meshes/Scifi_Character03/Scifi_Character03_Hair.Scifi_Character03_Hair'"));
	Hair->SetSkeletalMesh(mesh);
	
	// UpperBody
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &UpperBody, TEXT("UpperBody"), GetMesh());
	CHelpers::GetAsset(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Runner/Meshes/Scifi_Character03/Scifi_Character03_TorsoA.Scifi_Character03_TorsoA'"));
	UpperBody->SetSkeletalMesh(mesh);
	
	// Outer
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Outer, TEXT("Outer"), GetMesh());
	CHelpers::GetAsset(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Runner/Meshes/Scifi_Character03/Scifi_Character03_TorsoB.Scifi_Character03_TorsoB'"));
	Outer->SetSkeletalMesh(mesh);
	
	// Arms
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Arms, TEXT("Arms"), GetMesh());
	CHelpers::GetAsset(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Runner/Meshes/Scifi_Character02/Scifi_Character02_Arms.Scifi_Character02_Arms'"));
	Arms->SetSkeletalMesh(mesh);
	
	// LowerBody
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &LowerBody, TEXT("LowerBody"), GetMesh());
	CHelpers::GetAsset(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Runner/Meshes/Scifi_Character04/Scifi_Character04_Legs.Scifi_Character04_Legs'"));
	LowerBody->SetSkeletalMesh(mesh);
	
	// Foot
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Foot, TEXT("Foot"), GetMesh());
	CHelpers::GetAsset(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Runner/Meshes/Scifi_Character03/Scifi_Character03_Foot.Scifi_Character03_Foot'"));
	Foot->SetSkeletalMesh(mesh);
	
	// ChestRig
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &ChestRig, TEXT("ChestRig"), GetMesh());
	
	// Backpack
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Backpack, TEXT("Backpack"), GetMesh());

	// Character AnimInstance
	TSubclassOf<UCNoxAnimInstance> animInstance;
	CHelpers::GetClass<UCNoxAnimInstance>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/Characters/Runner/ABP_CNoxAnimInstance_Runner.ABP_CNoxAnimInstance_Runner_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);
	
	// FPSArms
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &FPSArms, TEXT("FPSArms"), CameraComponent);
	FPSArms->SetSkeletalMesh(Arms->GetSkeletalMeshAsset());
	FPSArms->SetRelativeLocation(FVector(-14.25f, -5.85f, -156.935f));
	FPSArms->SetRelativeRotation(FRotator(-0.5f, -11.85f, -1.2f));
	FPSArms->SetVisibility(false);

	// FPS AnimInstance
	CHelpers::GetClass<UCNoxAnimInstance>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/Characters/Runner/ABP_CNoxAnimInstance_RunnerArms.ABP_CNoxAnimInstance_RunnerArms_C'");
	FPSArms->SetAnimInstanceClass(animInstance);
}

void ACNox_Runner::InitCharacterMovement()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}
#pragma endregion

#pragma region Custom Component
void ACNox_Runner::InitCustomComponents()
{
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &MovementComponent, TEXT("MovementComponent"));
	
	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &WeaponComponent, TEXT("WeaponComponent"));
}
#pragma endregion

#pragma region EnhancedInput
void ACNox_Runner::InitMappingContexts()
{
	CHelpers::GetAsset<UInputMappingContext>(&IMC_Runner,TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IMC_Runner.IMC_Runner'"));
}
#pragma endregion
