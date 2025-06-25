#include "Items/Equipments/Weapons/Ranges/CWeapon_Range_AR.h"
#include "Global.h"
#include "Characters/CNox.h"
#include "Characters/Runner/CNox_Runner.h"
#include "Components/SkeletalMeshComponent.h"

ACWeapon_Range_AR::ACWeapon_Range_AR()
{
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SightMesh, "Sight", Mesh, "aim");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &ButtMesh, "Butt", Mesh, "butt");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &MagazineMesh, "Magazine", Mesh, "magazine");

	// Socket
	{
		LeftHandSocketName = "LeftHand_AR";
	}

	//Equip
	{
		HolsterSocketName = "Holster_AR";
		CHelpers::GetAsset<UAnimMontage>(&EquipMontage, "");
		EquipMontage_PlayRate = 2;
		RightHandSocketName = "RightHand_AR";
		LeftHandLocation = FVector(-35, 15.5f, 4);
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
		RecoilAngle = 0.75f;
		// CHelpers::GetClass<UMatineeCameraShake>(&CameraShakeClass, "Blueprint'/Game/Weapons/BP_CameraShake_AK47.BP_CameraShake_AK47_C'");
		AutoFireInterval = 0.1f;
		RecoilRate = 0.05f;
		SpreadSpeed = 2.0f;
		MaxSpreadAlignment = 2.0f;
	}

	// //UI
	// {
	// 	CHelpers::GetClass<UCUserWidget_CrossHair>(&CrossHairClass, "WidgetBlueprint'/Game/Widgets/WB_CrossHair.WB_CrossHair_C'");
	// }

	//Magazine
	{
		MaxMagazineCount = 30;
		MagazineBoneName = "b_gun_mag";
		MagazineSocketName = "Rifle_Magazine";
	}

	//Arms
	{
		ArmsMeshTransform.SetLocation(FVector(-14.25f, -5.85f, -156.935f));
		ArmsMeshTransform.SetRotation(FQuat(FRotator(-0.5f, -11.85f, -1.2f)));

		ArmsLeftHandTransform.SetLocation(FVector(-33, 11, -1.5f));
		ArmsLeftHandTransform.SetRotation(FQuat(FRotator(-4, -138, 77)));
	}
}

void ACWeapon_Range_AR::Begin_Equip()
{
	if (LeftHandSocketName.IsValid())
		CHelpers::AttachTo(this, OwnerCharacter->GetMesh(), LeftHandSocketName);

	OwnerCharacter->GetFPSArms()->SetRelativeTransform(ArmsMeshTransform);
}

void ACWeapon_Range_AR::End_Equip()
{
	Super::Begin_Equip();
	Super::End_Equip();
}

void ACWeapon_Range_AR::Begin_Aim()
{
	Super::Begin_Aim();

	// if (CrossHair)
	// 	CrossHair->SetVisibility(ESlateVisibility::Hidden);
	//
	// OwnerCharacter->GetMesh()->SetVisibility(false);
	// OwnerCharacter->GetBackpack()->SetVisibility(false);
	// OwnerCharacter->GetArms()->SetVisibility(true);
	//
	// CHelpers::AttachTo(this, OwnerCharacter->GetArms(), RightHandSocketName);
	//
	// CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter)->OnWeaponAim_Arms_Begin.Broadcast(this);
}

void ACWeapon_Range_AR::End_Aim()
{
	Super::End_Aim();

	// if (CrossHair)
	// 	CrossHair->SetVisibility(ESlateVisibility::Visible);
	//
	// OwnerCharacter->GetMesh()->SetVisibility(true);
	// OwnerCharacter->GetBackpack()->SetVisibility(true);
	// OwnerCharacter->GetArms()->SetVisibility(false);
	//
	// CHelpers::AttachTo(this, OwnerCharacter->GetMesh(), RightHandSocketName);
	//
	// CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter)->OnWeaponAim_Arms_End.Broadcast();
}
