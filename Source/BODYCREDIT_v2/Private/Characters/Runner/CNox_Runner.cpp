#include "Characters/Runner/CNox_Runner.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/CNoxAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Runner/CMovementComponent.h"
#include "Components/Runner/CWeaponComponent.h"
#include "Components/Market/CMarketComponent.h"
#include "Components/Inventory/CInventoryComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Games/CNoxController.h"
#include "Characters/Runner/CNox_RenderTarget.h"

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
	
	// RenderTarget이 설정되어 있으면 주기적으로 동기화 체크
	if (RenderTarget)
	{
		CheckAndSyncMeshes();
	}
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
	CHelpers::GetAsset(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Runner/Meshes/Scifi_Character01/Scifi_Character01_Bag.Scifi_Character01_Bag'"));
	Backpack->SetSkeletalMesh(mesh);

	//WeaponSlot1
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &WeaponSlot1, TEXT("WeaponSlot1"), GetMesh());
	WeaponSlot1->SetHiddenInGame(true);

	//WeaponSlot1
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &WeaponSlot2, TEXT("WeaponSlot2"), GetMesh());
	WeaponSlot2->SetHiddenInGame(true);

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

	CHelpers::CreateActorComponent<UCMarketComponent>(this, &MarketComponent, TEXT("MarketComp"));

	CHelpers::CreateActorComponent<UCInventoryComponent>(this, &InventoryComponent, TEXT("InventoryComponent"));
}
#pragma endregion

#pragma region EnhancedInput
void ACNox_Runner::InitMappingContexts()
{
	CHelpers::GetAsset<UInputMappingContext>(&IMC_Runner,TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IMC_Runner.IMC_Runner'"));
}
#pragma endregion

#pragma region RenderTarget Sync
void ACNox_Runner::SetRenderTarget(ACNox_RenderTarget* InRenderTarget)
{
	RenderTarget = InRenderTarget;
	
	// RenderTarget이 설정되면 즉시 동기화
	if (RenderTarget)
	{
		SyncMeshToRenderTarget();
		// 델리게이트 바인딩
		BindMeshChangeDelegates();
	}
}

void ACNox_Runner::BindMeshChangeDelegates()
{
	if (!RenderTarget)
	{
		return;
	}

	// 각 메시 컴포넌트에 대한 변경 감지 델리게이트 바인딩
	// 실제로는 UE5에서 메시 변경을 직접 감지하는 델리게이트가 제한적이므로
	// 주기적으로 체크하거나 특정 이벤트에서 동기화를 호출하는 방식으로 구현
	UE_LOG(LogTemp, Log, TEXT("ACNox_Runner::BindMeshChangeDelegates - Mesh change delegates bound."));
}

void ACNox_Runner::OnMeshChanged(USkeletalMeshComponent* ChangedComponent)
{
	if (!RenderTarget || !ChangedComponent)
	{
		return;
	}

	// 메시 변경 플래그 설정
	bMeshChanged = true;

	// 변경된 컴포넌트에 해당하는 RenderTarget 컴포넌트 찾기
	USkeletalMeshComponent* TargetComponent = nullptr;
	
	if (ChangedComponent == GetMesh())
	{
		TargetComponent = RenderTarget->GetMesh();
	}
	else if (ChangedComponent == Hair)
	{
		TargetComponent = RenderTarget->GetHairMesh();
	}
	else if (ChangedComponent == UpperBody)
	{
		TargetComponent = RenderTarget->GetUpperBodyMesh();
	}
	else if (ChangedComponent == Outer)
	{
		TargetComponent = RenderTarget->GetOuterMesh();
	}
	else if (ChangedComponent == Arms)
	{
		TargetComponent = RenderTarget->GetArmsMesh();
	}
	else if (ChangedComponent == LowerBody)
	{
		TargetComponent = RenderTarget->GetLowerBodyMesh();
	}
	else if (ChangedComponent == Foot)
	{
		TargetComponent = RenderTarget->GetFootMesh();
	}
	else if (ChangedComponent == ChestRig)
	{
		TargetComponent = RenderTarget->GetChestRigMesh();
	}
	else if (ChangedComponent == Backpack)
	{
		TargetComponent = RenderTarget->GetBackpackMesh();
	}
	else if (ChangedComponent == FPSArms)
	{
		TargetComponent = RenderTarget->GetFPSArmsMesh();
	}
	else if (ChangedComponent == WeaponSlot1)
	{
		TargetComponent = RenderTarget->GetWeaponSlot1Mesh();
	}
	else if (ChangedComponent == WeaponSlot2)
	{
		TargetComponent = RenderTarget->GetWeaponSlot2Mesh();
	}
	
	// 해당하는 컴포넌트가 있으면 즉시 동기화
	if (TargetComponent)
	{
		SyncMeshComponent(ChangedComponent, TargetComponent);
		UE_LOG(LogTemp, Log, TEXT("ACNox_Runner::OnMeshChanged - Auto-synced mesh component."));
	}
}

void ACNox_Runner::SyncMeshToRenderTarget()
{
	if (!RenderTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACNox_Runner::SyncMeshToRenderTarget - RenderTarget is null."));
		return;
	}

	// 메인 메시 동기화
	SyncMeshComponent(GetMesh(), RenderTarget->GetMesh());
	
	// 각 부위별 메시 동기화
	SyncMeshComponent(Hair, RenderTarget->GetHairMesh());
	SyncMeshComponent(UpperBody, RenderTarget->GetUpperBodyMesh());
	SyncMeshComponent(Outer, RenderTarget->GetOuterMesh());
	SyncMeshComponent(Arms, RenderTarget->GetArmsMesh());
	SyncMeshComponent(LowerBody, RenderTarget->GetLowerBodyMesh());
	SyncMeshComponent(Foot, RenderTarget->GetFootMesh());
	SyncMeshComponent(ChestRig, RenderTarget->GetChestRigMesh());
	SyncMeshComponent(Backpack, RenderTarget->GetBackpackMesh());
	SyncMeshComponent(FPSArms, RenderTarget->GetFPSArmsMesh());
	SyncMeshComponent(WeaponSlot1, RenderTarget->GetWeaponSlot1Mesh());
	SyncMeshComponent(WeaponSlot2, RenderTarget->GetWeaponSlot2Mesh());
	
	UE_LOG(LogTemp, Log, TEXT("ACNox_Runner::SyncMeshToRenderTarget - Mesh synchronization completed."));
}

void ACNox_Runner::SyncMeshComponent(USkeletalMeshComponent* SourceComponent, USkeletalMeshComponent* TargetComponent)
{
	if (!SourceComponent || !TargetComponent)
	{
		return;
	}

	// 스켈레탈 메시 동기화
	USkeletalMesh* SourceMesh = SourceComponent->GetSkeletalMeshAsset();
	if (SourceMesh)
	{
		TargetComponent->SetSkeletalMesh(SourceMesh);
	}
	
	// 머티리얼 동기화
	for (int32 i = 0; i < SourceComponent->GetNumMaterials(); ++i)
	{
		UMaterialInterface* Material = SourceComponent->GetMaterial(i);
		if (Material)
		{
			TargetComponent->SetMaterial(i, Material);
		}
	}
	
	// 가시성 동기화
	TargetComponent->SetVisibility(SourceComponent->IsVisible());
	TargetComponent->SetHiddenInGame(SourceComponent->bHiddenInGame);
}

// 각 메시 컴포넌트별 변경 함수들
void ACNox_Runner::SetHairMesh(USkeletalMesh* NewMesh)
{
	if (Hair)
	{
		Hair->SetSkeletalMesh(NewMesh);
		OnMeshChanged(Hair);
	}
}

void ACNox_Runner::SetUpperBodyMesh(USkeletalMesh* NewMesh)
{
	if (UpperBody)
	{
		UpperBody->SetSkeletalMesh(NewMesh);
		OnMeshChanged(UpperBody);
	}
}

void ACNox_Runner::SetOuterMesh(USkeletalMesh* NewMesh)
{
	if (Outer)
	{
		Outer->SetSkeletalMesh(NewMesh);
		OnMeshChanged(Outer);
	}
}

void ACNox_Runner::SetArmsMesh(USkeletalMesh* NewMesh)
{
	if (Arms)
	{
		Arms->SetSkeletalMesh(NewMesh);
		OnMeshChanged(Arms);
	}
}

void ACNox_Runner::SetLowerBodyMesh(USkeletalMesh* NewMesh)
{
	if (LowerBody)
	{
		LowerBody->SetSkeletalMesh(NewMesh);
		OnMeshChanged(LowerBody);
	}
}

void ACNox_Runner::SetFootMesh(USkeletalMesh* NewMesh)
{
	if (Foot)
	{
		Foot->SetSkeletalMesh(NewMesh);
		OnMeshChanged(Foot);
	}
}

void ACNox_Runner::SetChestRigMesh(USkeletalMesh* NewMesh)
{
	if (ChestRig)
	{
		ChestRig->SetSkeletalMesh(NewMesh);
		OnMeshChanged(ChestRig);
	}
}

void ACNox_Runner::SetBackpackMesh(USkeletalMesh* NewMesh)
{
	if (Backpack)
	{
		Backpack->SetSkeletalMesh(NewMesh);
		OnMeshChanged(Backpack);
	}
}

void ACNox_Runner::SetFPSArmsMesh(USkeletalMesh* NewMesh)
{
	if (FPSArms)
	{
		FPSArms->SetSkeletalMesh(NewMesh);
		OnMeshChanged(FPSArms);
	}
}

void ACNox_Runner::SetWeaponSlot1Mesh(USkeletalMesh* NewMesh)
{
	if (WeaponSlot1)
	{
		WeaponSlot1->SetSkeletalMesh(NewMesh);
		OnMeshChanged(WeaponSlot1);
	}
}

void ACNox_Runner::SetWeaponSlot2Mesh(USkeletalMesh* NewMesh)
{
	if (WeaponSlot2)
	{
		WeaponSlot2->SetSkeletalMesh(NewMesh);
		OnMeshChanged(WeaponSlot2);
	}
}

void ACNox_Runner::TestRenderTargetSync()
{
	// RenderTarget이 설정되어 있는지 확인
	if (!RenderTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACNox_Runner::TestRenderTargetSync - RenderTarget is not set. Please set RenderTarget first."));
		return;
	}
	
	// 전체 동기화 실행
	SyncMeshToRenderTarget();
	
	UE_LOG(LogTemp, Log, TEXT("ACNox_Runner::TestRenderTargetSync - RenderTarget sync test completed successfully."));
}

void ACNox_Runner::CheckAndSyncMeshes()
{
	if (!RenderTarget)
	{
		return;
	}

	// 현재 시간 체크
	float CurrentTime = GetWorld()->GetTimeSeconds();
	
	// 동기화 체크 간격이 지났고 메시가 변경되었으면 동기화 실행
	if (CurrentTime - LastSyncTime >= SyncCheckInterval && bMeshChanged)
	{
		LastSyncTime = CurrentTime;
		bMeshChanged = false; // 플래그 리셋
		
		// 전체 동기화 실행
		SyncMeshToRenderTarget();
		UE_LOG(LogTemp, Log, TEXT("ACNox_Runner::CheckAndSyncMeshes - Periodic sync completed."));
	}
}
#pragma endregion
