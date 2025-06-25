#pragma once

#include "CoreMinimal.h"
#include "Components/CBaseComponent.h"
#include "CWeaponComponent.generated.h"

class ACWeapon_Range;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AR, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponAim_Arms_Begin, class ACWeapon_Range*, InThisWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponAim_Arms_End);

UCLASS()
class BODYCREDIT_V2_API UCWeaponComponent : public UCBaseComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsARMode() { return Type == EWeaponType::AR; }

	void SetUnarmedMode();
	void SetARMode();

	void Begin_Equip();
	void End_Equip();

	void ToggleAutoFire();

	void Begin_Aim();
	void End_Aim();

	void Begin_Fire();
	void End_Fire();

	bool IsInAim();
	FVector GetLeftHandLocation();
	FTransform GetLeftHandTransform();

	void Reload();
	void Eject_Magazine();
	void Spawn_Magazine();
	void Load_Magazine();
	void End_Reload();

	FWeaponTypeChanged OnWeaponTypeChanged;
	FWeaponAim_Arms_Begin OnWeaponAim_Arms_Begin;
	FWeaponAim_Arms_End OnWeaponAim_Arms_End;

	UCWeaponComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ACWeapon_Range>> WeaponClasses;

	// UPROPERTY(EditAnywhere, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	// TSubclassOf<class UCUserWidget_HUD> HUDClass;

	// UCUserWidget_HUD* HUD;

	TArray<ACWeapon_Range*> Weapons;

	ACWeapon_Range* GetCurrWeapon();

	EWeaponType Type = EWeaponType::Max;

	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

	UFUNCTION()
	void On_Begin_Aim(ACWeapon_Range* InThisWeapon);

	UFUNCTION()
	void On_Begin_End();
};
