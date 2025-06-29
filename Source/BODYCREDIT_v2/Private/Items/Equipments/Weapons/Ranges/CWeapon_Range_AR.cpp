#include "Items/Equipments/Weapons/Ranges/CWeapon_Range_AR.h"
#include "Global.h"
#include "Camera/CameraComponent.h"
#include "Characters/Runner/CNox_Runner.h"
#include "Items/Equipments/Weapons/Ranges/CMagazine.h"
#include "Components/SkeletalMeshComponent.h"
#include "Widgets/Runners/CUserWidget_CrossHair.h"
#include "Components/Runner/CWeaponComponent.h"

ACWeapon_Range_AR::ACWeapon_Range_AR()
{
	USkeletalMesh* asset;
	CHelpers::GetAsset(&asset, TEXT("/Script/Engine.SkeletalMesh'/Game/Items/Equipments/Weapons/Ranges/ARs/Assault_AK/SK_Rifle_Mesh/SK_Rifle_Assault_Rifle.SK_Rifle_Assault_Rifle'"));
	
	// Mesh	
	Mesh->SetSkeletalMesh(asset);

	// Sight
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SightMesh, "Sight", Mesh, "scope");
	CHelpers::GetAsset(&asset, TEXT("/Script/Engine.SkeletalMesh'/Game/Items/Equipments/Weapons/Ranges/ARs/Assault_AK/SK_Rifle_Mesh/SK_Aim.SK_Aim'"));
	SightMesh->SetSkeletalMesh(asset);
	SightMesh->SetRelativeScale3D(FVector(1, 0.95f, 1));
	SightMesh->SetCollisionProfileName("NoCollision");
	
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &ButtMesh, "Butt", Mesh, "butt");
	CHelpers::GetAsset(&asset, TEXT("/Script/Engine.SkeletalMesh'/Game/Items/Equipments/Weapons/Ranges/ARs/Assault_AK/SK_Rifle_Mesh/SK_Butt.SK_Butt'"));
	ButtMesh->SetSkeletalMesh(asset);
	
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &MagazineMesh, "Magazine", Mesh, "magazine");
	CHelpers::GetAsset(&asset, TEXT("/Script/Engine.SkeletalMesh'/Game/Items/Equipments/Weapons/Ranges/ARs/Assault_AK/SK_Rifle_Mesh/SK_Magazine.SK_Magazine'"));
	MagazineMesh->SetSkeletalMesh(asset);

	// Socket
	{
		LeftHandSocketName = "LeftHand_AR";
	}

	//Equip
	{
		HolsterSocketName = "Holster_AR";
		CHelpers::GetAsset<UAnimMontage>(&EquipMontage, "/Script/Engine.AnimMontage'/Game/Characters/Runner/Animations/AR/Rifle_Equip_AK47_Montage.Rifle_Equip_AK47_Montage'");
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
		AimData.FieldOfView = 75;
	}

	//Fire
	{
		CHelpers::GetAsset<USoundWave>(&FireSound, "/Script/Engine.SoundWave'/Game/Items/Equipments/Weapons/Ranges/Sounds/AK_Shoot.AK_Shoot'");
		RecoilAngle = 1.0f;
		CHelpers::GetClass<UCameraShakeBase>(&CameraShakeClass, "/Script/Engine.Blueprint'/Game/Items/Equipments/Weapons/Ranges/ARs/BP_CameraShake_AR.BP_CameraShake_AR_C'");
		AutoFireInterval = 0.1f;
		RecoilRate = 0.5f;
		SpreadSpeed = 4.0f;
		MaxSpreadAlignment = 2.0f;
	}

	// //UI
	{
		CHelpers::GetClass<UCUserWidget_CrossHair>(&CrossHairClass, "/Script/UMGEditor.WidgetBlueprint'/Game/UIs/Runner/WBP_CrossHair.WBP_CrossHair_C'");
	}

	//Magazine
	{
		MaxMagazineCount = 30;
		CHelpers::GetAsset<UAnimMontage>(&ReloadMontage, "/Script/Engine.AnimMontage'/Game/Characters/Runner/Animations/AR/Rifle_Reload_Montage.Rifle_Reload_Montage'");
		ReloadMontage_PlayRate = 1.5f;
		MagazineBoneName = "magazine";
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

void ACWeapon_Range_AR::BeginPlay()
{
	Super::BeginPlay();

	if (Mesh)
	{
		MeshMID = Mesh->CreateAndSetMaterialInstanceDynamic(0);
		SyncSightAmmoCount();
	}

	if (SightMesh)
	{
		SightMID = SightMesh->CreateAndSetMaterialInstanceDynamic(0);
		SyncSightAmmoCount();
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

void ACWeapon_Range_AR::End_Aim()
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

void ACWeapon_Range_AR::OnFiring()
{
	Super::OnFiring();
	
	SyncSightAmmoCount();
}

void ACWeapon_Range_AR::SyncSightAmmoCount()
{
	if (MeshMID)
	{
		// 원래 정규화: float phase = (float)CurrMagazineCount / MaxMagazineCount;
		// 변경: 정규화 제거하고 실제 카운트 값 그대로 반영
		float ammoCountValue = (float)CurrMagazineCount;
		MeshMID->SetScalarParameterValue(FName("Ammo Count"), ammoCountValue);

		UE_LOG(LogTemp, Warning, TEXT("Ammo Count Set: %d"), CurrMagazineCount);
	}

	if (SightMID)
	{
		// 원래 정규화: float phase = (float)CurrMagazineCount / MaxMagazineCount;
		// 변경: 정규화 제거하고 실제 카운트 값 그대로 반영
		float ammoCountValue = (float)CurrMagazineCount;
		SightMID->SetScalarParameterValue(FName("Ammo Count"), ammoCountValue);

		UE_LOG(LogTemp, Warning, TEXT("Ammo Count Set: %d"), CurrMagazineCount);
	}
}

void ACWeapon_Range_AR::Load_Magazine()
{
	Super::Load_Magazine();
	
	SyncSightAmmoCount();
}
