#include "Items/Equipments/Weapons/Ranges/CWeapon_Range_Magnum.h"
#include "Global.h"
#include "Camera/CameraShakeBase.h"
#include "Characters/Runner/CNox_Runner.h"
#include "Widgets/Runners/CUserWidget_CrossHair.h"
#include "Components/Runner/CWeaponComponent.h"
#include "Items/Equipments/Weapons/Ranges/CMagazine.h"

ACWeapon_Range_Magnum::ACWeapon_Range_Magnum()
{
	//Equip
	{
		HolsterSocketName = "Holster_Pistol";
		 CHelpers::GetAsset<UAnimMontage>(&EquipMontage, "/Script/Engine.AnimMontage'/Game/Characters/Runner/Animations/Pistol/Runner_Pistol_Equip_Montage.Runner_Pistol_Equip_Montage'");
		EquipMontage_PlayRate = 2;
		RightHandSocketName = "RightHand_Pistol";
		LeftHandLocation = FVector(0, 12.5f, 0);
	}

	//Aim
	{
		BaseData.bEnableCameraLag = true;
		BaseData.TargetArmLength = 200;
		BaseData.SocketOffset = FVector(0, 50, 15);
		BaseData.FieldOfView = 90;

		AimData.bEnableCameraLag = false;
		AimData.TargetArmLength = 30;
		AimData.SocketOffset = FVector(-55, 0, 10);
		AimData.FieldOfView = 55;
	}

	//Fire
	{
		CHelpers::GetAsset<USoundWave>(&FireSound, "/Script/Engine.SoundWave'/Game/Items/Equipments/Weapons/Ranges/Sounds/Pistol_Shoot.Pistol_Shoot'");
		RecoilAngle = 1.5f;
		// CHelpers::GetClass<UCameraShakeBase>(&CameraShakeClass, "Blueprint'/Game/Weapons/BP_CameraShake_Pistol.BP_CameraShake_Pistol_C'");
		AutoFireInterval = 0.3f;
		RecoilRate = 0.05f;
		SpreadSpeed = 6;
		MaxSpreadAlignment = 4;
	}

	//UI
	{
		CHelpers::GetClass<UCUserWidget_CrossHair>(&CrossHairClass, "/Script/UMGEditor.WidgetBlueprint'/Game/UIs/Runner/WBP_CrossHair.WBP_CrossHair_C'");
	}

	//Magazine
	{
		MaxMagazineCount = 6;
		 CHelpers::GetAsset<UAnimMontage>(&ReloadMontage, "/Script/Engine.AnimMontage'/Game/Characters/Runner/Animations/Pistol/Runner_Pistol_Reload_Montage.Runner_Pistol_Reload_Montage'");
		ReloadMontage_PlayRate = 1.5f;
		MagazineBoneName = NAME_None;
		 CHelpers::GetClass<ACMagazine>(&MagazineClass, "/Script/Engine.Blueprint'/Game/Items/Equipments/Weapons/Ranges/Magazines/BP_CMagazine_Pistol.BP_CMagazine_Pistol_C'");
		MagazineSocketName = "Pistol_Magazine";
	}

	//Arms
	{
		ArmsMeshTransform.SetLocation(FVector(0, 5.1f, -156.6));
		ArmsMeshTransform.SetRotation(FQuat(FRotator(0, -4.8f, 0)));

		ArmsLeftHandTransform.SetLocation(FVector(0, 10, 0));
		ArmsLeftHandTransform.SetRotation(FQuat(FRotator(0, 180, 180)));
	}
}

void ACWeapon_Range_Magnum::BeginPlay()
{
	Super::BeginPlay();
	
	const int32 Num = Mesh->GetNumMaterials();
	for (int32 i = 0; i < Num; ++i)
	{
		if (UMaterialInstanceDynamic* Dyn = Mesh->CreateAndSetMaterialInstanceDynamic(i))
		{
			MaterialMIDs.Add(Dyn);
		}
	}
	Load_Magazine();
	
	Mesh->SetVisibility(false);
}

void ACWeapon_Range_Magnum::Begin_Equip()
{
	Super::Begin_Equip();

	Mesh->SetVisibility(true);
	OwnerCharacter->GetFPSArms()->SetRelativeTransform(ArmsMeshTransform);
}

void ACWeapon_Range_Magnum::Begin_Aim()
{
	Super::Begin_Aim();

	if (CrossHair) CrossHair->SetVisibility(ESlateVisibility::Hidden);

	OwnerCharacter->GetMesh()->SetVisibility(false);

	TArray<USceneComponent*> children;
	OwnerCharacter->GetMesh()->GetChildrenComponents(false, children);
	for (USceneComponent* child : children) child->SetVisibility(false);

	OwnerCharacter->GetFPSArms()->SetVisibility(true);

	CHelpers::AttachTo(this, OwnerCharacter->GetFPSArms(), RightHandSocketName);

	CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter)->OnWeaponAim_Arms_Begin.Broadcast(this);
}

void ACWeapon_Range_Magnum::End_Aim()
{
	Super::End_Aim();

	if (CrossHair) CrossHair->SetVisibility(ESlateVisibility::Visible);

	OwnerCharacter->GetMesh()->SetVisibility(true);

	TArray<USceneComponent*> children;
	OwnerCharacter->GetMesh()->GetChildrenComponents(false, children);
	for (USceneComponent* child : children) child->SetVisibility(true);

	OwnerCharacter->GetFPSArms()->SetVisibility(false);

	CHelpers::AttachTo(this, OwnerCharacter->GetMesh(), RightHandSocketName);

	CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter)->OnWeaponAim_Arms_End.Broadcast();
}

void ACWeapon_Range_Magnum::OnFiring()
{
	Super::OnFiring();
	
	// Decrement phase for each shot
	AmmoPhase += 0.09f;

	for (auto* MID : MaterialMIDs)
	{
		MID->SetScalarParameterValue(FName("AmmoPhase"), AmmoPhase);
	}
}

void ACWeapon_Range_Magnum::SyncSightAmmoCount()
{
	// Normalize ammo count to [0,1] phase
	float maxAmmo = static_cast<float>(MaxMagazineCount);
	float phase = (maxAmmo > 0.f) ? static_cast<float>(CurrMagazineCount) / maxAmmo : 0.f;

	for (auto* MID : MaterialMIDs)
	{
		MID->SetScalarParameterValue(FName("AmmoPhase"), phase);
	}
}

void ACWeapon_Range_Magnum::Load_Magazine()
{
	Super::Load_Magazine();
	
	// Reset phase when reloading
	AmmoPhase = 0.54f;;

	for (auto* MID : MaterialMIDs)
	{
		MID->SetScalarParameterValue(FName("AmmoPhase"), AmmoPhase);
	}
}
