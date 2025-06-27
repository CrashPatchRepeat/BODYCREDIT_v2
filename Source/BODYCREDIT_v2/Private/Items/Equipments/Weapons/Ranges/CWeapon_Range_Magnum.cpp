#include "Items/Equipments/Weapons/Ranges/CWeapon_Range_Magnum.h"
#include "Global.h"
#include "Camera/CameraShakeBase.h"
#include "Characters/Runner/CNox_Runner.h"
#include "Widgets/Runners/CUserWidget_CrossHair.h"
#include "Components/Runner/CWeaponComponent.h"

ACWeapon_Range_Magnum::ACWeapon_Range_Magnum()
{
	//Equip
	{
		HolsterSocketName = NAME_None;
		// CHelpers::GetAsset<UAnimMontage>(&EquipMontage, "");
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
		RecoilAngle = 1.5f;
		// CHelpers::GetClass<UCameraShakeBase>(&CameraShakeClass, "Blueprint'/Game/Weapons/BP_CameraShake_Pistol.BP_CameraShake_Pistol_C'");
		AutoFireInterval = 0.3f;
		RecoilRate = 0.05f;
		SpreadSpeed = 6;
		MaxSpreadAlignment = 4;
	}

	//UI
	{
		// CHelpers::GetClass<UCUserWidget_CrossHair>(&CrossHairClass, "WidgetBlueprint'/Game/Widgets/WB_CrossHair.WB_CrossHair_C'");
	}

	//Magazine
	{
		MaxMagazineCount = 5;
		// CHelpers::GetAsset<UAnimMontage>(&ReloadMontage, "AnimMontage'/Game/Character/Animations/Pistol_Reload_Montage.Pistol_Reload_Montage'");
		ReloadMontage_PlayRate = 1.5f;
		MagazineBoneName = NAME_None;
		// CHelpers::GetClass<ACMagazine>(&MagazineClass, "Blueprint'/Game/Weapons/BP_CMagazine_Pistol.BP_CMagazine_Pistol_C'");
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
