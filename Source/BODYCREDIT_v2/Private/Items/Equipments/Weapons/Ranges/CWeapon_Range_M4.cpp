#include "Items/Equipments/Weapons/Ranges/CWeapon_Range_M4.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Widgets/Runners/CUserWidget_CrossHair.h"
#include "Characters/Runner/CNox_Runner.h"
#include "Components/Runner/CWeaponComponent.h"
#include "Items/Equipments/Weapons/Ranges/CMagazine.h"

ACWeapon_Range_M4::ACWeapon_Range_M4()
{
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SightMesh, "Sight", Mesh, "aim");
	SightMesh->SetCollisionProfileName("NoCollision");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &GlassMesh, "Glass", SightMesh, "glass_slot");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &ReactoreMesh, "Reactor", Mesh, "reactor");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &ButtMesh, "Butt", Mesh, "butt_slot");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &ForegripMesh, "Foregrip", Mesh, "foregrip_slot");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &MagazineMesh, "Magazine", Mesh, "magazine_slot");

	// Socket
	{
		LeftHandSocketName = "LeftHand_AR";
	}

	//Equip
	{
		HolsterSocketName = "Holster_AR";
		CHelpers::GetAsset<UAnimMontage>(&EquipMontage, "/Script/Engine.AnimMontage'/Game/Characters/Runner/Animations/AR/Rifle_Equip_AK47_Montage.Rifle_Equip_AK47_Montage'");
		EquipMontage_PlayRate = 2;
		RightHandSocketName = "RightHand_M4";
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
		AimData.FieldOfView = 75;
	}

	//Fire
	{
		CHelpers::GetAsset<USoundWave>(&FireSound, "/Script/Engine.SoundWave'/Game/Items/Equipments/Weapons/Ranges/Sounds/M4_Shoot.M4_Shoot'");
		RecoilAngle = 0.75f;
		CHelpers::GetClass<UCameraShakeBase>(&CameraShakeClass, "/Script/Engine.Blueprint'/Game/Items/Equipments/Weapons/Ranges/ARs/BP_CameraShake_AR.BP_CameraShake_AR_C'");
		AutoFireInterval = 0.08f;
		RecoilRate = 0.2f;
		SpreadSpeed = 3.0f;
		MaxSpreadAlignment = 2.0f;
	}

	// //UI
	{
		CHelpers::GetClass<UCUserWidget_CrossHair>(&CrossHairClass, "/Script/UMGEditor.WidgetBlueprint'/Game/Wigets/Runner/WBP_CrossHair.WBP_CrossHair_C'");
	}

	//Magazine
	{
		MaxMagazineCount = 30;
		CHelpers::GetAsset<UAnimMontage>(&ReloadMontage, "/Script/Engine.AnimMontage'/Game/Characters/Runner/Animations/AR/Rifle_Reload_Montage.Rifle_Reload_Montage'");
		ReloadMontage_PlayRate = 1.5f;
		MagazineBoneName = "magazine_slot";
		CHelpers::GetClass<ACMagazine>(&MagazineClass, "/Script/Engine.Blueprint'/Game/Items/Equipments/Weapons/Ranges/Magazines/BP_CMagazine_AK.BP_CMagazine_AK_C'");
		MagazineSocketName = "AR_Magazine";
	}

	//Arms
	{
		ArmsMeshTransform.SetLocation(FVector(-23.0f, -5.9f, -156.935f));
		ArmsMeshTransform.SetRotation(FQuat(FRotator(-0.5f, -11.85f, -1.2f)));

		ArmsLeftHandTransform.SetLocation(FVector(-33, 11, -1.5f));
		ArmsLeftHandTransform.SetRotation(FQuat(FRotator(-4, -138, 77)));
	}
}

void ACWeapon_Range_M4::Begin_Equip()
{
	if (LeftHandSocketName.IsValid())
		CHelpers::AttachTo(this, OwnerCharacter->GetMesh(), LeftHandSocketName);

	OwnerCharacter->GetFPSArms()->SetRelativeTransform(ArmsMeshTransform);
}

void ACWeapon_Range_M4::End_Equip()
{
	Super::Begin_Equip();
	Super::End_Equip();
}

void ACWeapon_Range_M4::Begin_Aim()
{
	Super::Begin_Aim();

	if (CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Hidden);
	
	OwnerCharacter->GetMesh()->SetVisibility(false);
	
	TArray<USceneComponent*> children;
	OwnerCharacter->GetMesh()->GetChildrenComponents(false, children);
	for (USceneComponent* child : children)
		child->SetVisibility(false);
	
	OwnerCharacter->GetFPSArms()->SetVisibility(true);
	
	CHelpers::AttachTo(this, OwnerCharacter->GetFPSArms(), RightHandSocketName);
	
	CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter)->OnWeaponAim_Arms_Begin.Broadcast(this);
}

void ACWeapon_Range_M4::End_Aim()
{
	Super::End_Aim();

	if (CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Visible);
	
	OwnerCharacter->GetMesh()->SetVisibility(true);
	
	TArray<USceneComponent*> children;
	OwnerCharacter->GetMesh()->GetChildrenComponents(false, children);
	for (USceneComponent* child : children)
		child->SetVisibility(true);
	
	OwnerCharacter->GetFPSArms()->SetVisibility(false);
	
	CHelpers::AttachTo(this, OwnerCharacter->GetMesh(), RightHandSocketName);
	
	CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter)->OnWeaponAim_Arms_End.Broadcast();
}
