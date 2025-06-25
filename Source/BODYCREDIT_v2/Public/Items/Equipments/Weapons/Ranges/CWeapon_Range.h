#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/Weapons/CEquipment_Weapon.h"
#include "CWeapon_Range.generated.h"

class ACNox;
class UAnimMontage;
class UTimelineComponent;
class UCurveFloat;
class UParticleSystem;
class USoundWave;
class UMaterialInstanceConstant;
class ACBullet;

USTRUCT()
struct FWeaponAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TargetArmLength;

	UPROPERTY(EditAnywhere)
	FVector SocketOffset;

	UPROPERTY(EditAnywhere)
	float FieldOfView;

	UPROPERTY(EditAnywhere)
	bool bEnableCameraLag;

public:
	void SetData(ACNox* InOwner);
	void SetDataByNoneCurve(ACNox* InOwner);
};

UCLASS()
class BODYCREDIT_V2_API ACWeapon_Range : public ACEquipment_Weapon
{
	GENERATED_BODY()

public:
#pragma region Equip
	FORCEINLINE FVector GetLeftHandLocation() { return LeftHandLocation; }
	FORCEINLINE FTransform GetArmsLeftHandTransform() { return ArmsLeftHandTransform; }
	
	bool CanEquip();
	void Equip();
	virtual void Begin_Equip();
	virtual void End_Equip();

	bool CanUnequip();
	void Unequip();
#pragma endregion

#pragma region Aim
	FORCEINLINE bool IsInAim() { return bInAim; }
	
	bool CanAim();
	virtual void Begin_Aim();
	virtual void End_Aim();
#pragma endregion

#pragma region Fire
	FORCEINLINE bool IsAutoFire() { return bAutoFire; }
	
	bool CanFire();
	void Begin_Fire();
	void End_Fire();
	
	void ToggleAutoFire();
#pragma endregion

#pragma region Reload
	FORCEINLINE uint8 GetCurrMagazineCount() { return CurrMagazineCount; }
	FORCEINLINE uint8 GetMaxMagazineCount() { return MaxMagazineCount; }
	
	bool CanReload();
	void Reload();

	void Eject_Magazine();
	void Spawn_Magazine();
	void Load_Magazine();
	void End_Reload();
#pragma endregion
	
	ACWeapon_Range();

	virtual void Tick(float DeltaTime) override;

protected:
#pragma region Equip
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName HolsterSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	float EquipMontage_PlayRate;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName RightHandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FVector LeftHandLocation;
	
	bool bEquipping = false;
#pragma endregion

#pragma region Aim
	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	UTimelineComponent* Timeline;
	
	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	FWeaponAimData BaseData;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	FWeaponAimData AimData;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	UCurveFloat* AimCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	float AimingSpeed = 200;
	
	bool bInAim = false;
#pragma endregion

#pragma region Fire
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	UParticleSystem* FlashParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	UParticleSystem* EjectParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	USoundWave* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float RecoilAngle;

	// UPROPERTY(EditDefaultsOnly, Category = "Fire")
	// TSubclassOf<class UMatineeCameraShake> CameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float AutoFireInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float RecoilRate;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float SpreadSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float MaxSpreadAlignment;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TSubclassOf<ACBullet> BulletClass;
	
	float CurrSpreadRadius;
	float LastAddSpreadTime;
	
	bool bAutoFire = true;
	bool bFiring = false;
#pragma endregion

#pragma region Hit
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	float HitDistance = 3000;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	UMaterialInstanceConstant* HitDecal;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	UParticleSystem* HitParticle;
#pragma endregion

#pragma region Reload
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magazine")
	class ACMagazine* Magazine;
	
	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
	TSubclassOf<class ACMagazine> MagazineClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
	uint8 MaxMagazineCount;

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
	float ReloadMontage_PlayRate;

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
	FName MagazineBoneName;


	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
	FName MagazineSocketName;

	uint8 CurrMagazineCount;
	
	bool bReload = false;
#pragma endregion

#pragma region Arms
	UPROPERTY(EditDefaultsOnly, Category = "Arms")
	FTransform ArmsMeshTransform;

	UPROPERTY(EditDefaultsOnly, Category = "Arms")
	FTransform ArmsLeftHandTransform;
#pragma endregion
	
	virtual void BeginPlay() override;

private:
#pragma region Aim
	UFUNCTION()
	void OnAiming(float Output);
#pragma endregion

#pragma region Fire
	FTimerHandle AutoFireHandle;
	
	UFUNCTION()
	void OnFiring();
#pragma endregion
};
