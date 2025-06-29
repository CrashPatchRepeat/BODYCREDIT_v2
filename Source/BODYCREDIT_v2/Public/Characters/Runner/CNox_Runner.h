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
class ACNox_RenderTarget;

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

#pragma region RenderTarget Sync
	// RenderTarget 참조 설정
	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	void SetRenderTarget(ACNox_RenderTarget* InRenderTarget);
	
	// 메시 동기화 함수
	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	void SyncMeshToRenderTarget();
	
	// 특정 메시 컴포넌트 동기화
	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	void SyncMeshComponent(USkeletalMeshComponent* SourceComponent, USkeletalMeshComponent* TargetComponent);
	
	// 메시 변경 시 자동 동기화
	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	void OnMeshChanged(USkeletalMeshComponent* ChangedComponent);
	
	// 메시 변경 감지를 위한 델리게이트 바인딩
	void BindMeshChangeDelegates();
	
	// 각 메시 컴포넌트별 변경 함수들
	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	void SetHairMesh(USkeletalMesh* NewMesh);
	
	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	void SetUpperBodyMesh(USkeletalMesh* NewMesh);
	
	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	void SetOuterMesh(USkeletalMesh* NewMesh);
	
	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	void SetArmsMesh(USkeletalMesh* NewMesh);
	
	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	void SetLowerBodyMesh(USkeletalMesh* NewMesh);
	
	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	void SetFootMesh(USkeletalMesh* NewMesh);
	
	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	void SetChestRigMesh(USkeletalMesh* NewMesh);
	
	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	void SetBackpackMesh(USkeletalMesh* NewMesh);
	
	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	void SetFPSArmsMesh(USkeletalMesh* NewMesh);
	
	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	void SetWeaponSlot1Mesh(USkeletalMesh* NewMesh);
	
	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	void SetWeaponSlot2Mesh(USkeletalMesh* NewMesh);
	
	// 테스트용 함수 - RenderTarget 연결 및 동기화 테스트
	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	void TestRenderTargetSync();
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSet", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponSlot1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSet", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponSlot2;
	
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

#pragma region RenderTarget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RenderTarget", meta = (AllowPrivateAccess = "true"))
	ACNox_RenderTarget* RenderTarget;
	
	// 동기화 체크를 위한 변수들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RenderTarget", meta = (AllowPrivateAccess = "true"))
	float SyncCheckInterval = 0.1f; // 0.1초마다 체크
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RenderTarget", meta = (AllowPrivateAccess = "true"))
	float LastSyncTime = 0.0f;
	
	// 메시 변경 감지를 위한 변수들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RenderTarget", meta = (AllowPrivateAccess = "true"))
	bool bMeshChanged = false;
	
	// Tick에서 동기화 체크
	void CheckAndSyncMeshes();
#pragma endregion
};
