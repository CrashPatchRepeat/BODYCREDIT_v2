#pragma once

#include "CoreMinimal.h"
#include "Characters/CNox.h"
#include "CNox_RenderTarget.generated.h"

UCLASS()
class BODYCREDIT_V2_API ACNox_RenderTarget : public ACNox
{
	GENERATED_BODY()

public:
	ACNox_RenderTarget();
	
	virtual void Tick(float DeltaTime) override;

#pragma region Mesh Accessors
	// 메시 컴포넌트들에 대한 접근자 함수들
	FORCEINLINE USkeletalMeshComponent* GetHairMesh() const { return Hair; }
	FORCEINLINE USkeletalMeshComponent* GetUpperBodyMesh() const { return UpperBody; }
	FORCEINLINE USkeletalMeshComponent* GetOuterMesh() const { return Outer; }
	FORCEINLINE USkeletalMeshComponent* GetArmsMesh() const { return Arms; }
	FORCEINLINE USkeletalMeshComponent* GetLowerBodyMesh() const { return LowerBody; }
	FORCEINLINE USkeletalMeshComponent* GetFootMesh() const { return Foot; }
	FORCEINLINE USkeletalMeshComponent* GetChestRigMesh() const { return ChestRig; }
	FORCEINLINE USkeletalMeshComponent* GetBackpackMesh() const { return Backpack; }
	FORCEINLINE USkeletalMeshComponent* GetFPSArmsMesh() const { return FPSArms; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponSlot1Mesh() const { return WeaponSlot1; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponSlot2Mesh() const { return WeaponSlot2; }
#pragma endregion

protected:
	virtual void BeginPlay() override;

private:
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
#pragma endregion
};
