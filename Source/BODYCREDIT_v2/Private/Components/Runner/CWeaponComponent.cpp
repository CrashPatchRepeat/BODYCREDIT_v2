#include "Components/Runner/CWeaponComponent.h"
#include "Global.h"
#include "Characters/Runner/CNox_Runner.h"
#include "Items/Equipments/Weapons/CEquipment_Weapon.h"
#include "items/Equipments/Weapons/Ranges/CWeapon_Range.h"

void UCWeaponComponent::SetUnarmedMode()
{
	CheckFalse(GetCurrWeapon()->CanUnequip());
	
	GetCurrWeapon()->Unequip();
	ChangeType(EWeaponType::Max);

	// if (HUD) HUD->SetVisibility(ESlateVisibility::Hidden);
}

void UCWeaponComponent::SetARMode()
{
	SetMode(EWeaponType::AR);
}

void UCWeaponComponent::Begin_Equip()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->Begin_Equip();
}

void UCWeaponComponent::End_Equip()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->End_Equip();
}

void UCWeaponComponent::ToggleAutoFire()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->ToggleAutoFire();
}

void UCWeaponComponent::Begin_Aim()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->Begin_Aim();
}

void UCWeaponComponent::End_Aim()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->End_Aim();
}

void UCWeaponComponent::Begin_Fire()
{
	CheckNull(GetCurrWeapon());
	CheckFalse(GetCurrWeapon()->CanFire());

	GetCurrWeapon()->Begin_Fire();
}

void UCWeaponComponent::End_Fire()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->End_Fire();
}

bool UCWeaponComponent::IsInAim()
{
	CheckNullResult(GetCurrWeapon(), false);

	return GetCurrWeapon()->IsInAim();
}

FVector UCWeaponComponent::GetLeftHandLocation()
{
	CheckNullResult(GetCurrWeapon(), FVector::ZeroVector);

	return GetCurrWeapon()->GetLeftHandLocation();
}

FTransform UCWeaponComponent::GetLeftHandTransform()
{
	CheckNullResult(GetCurrWeapon(), FTransform());

	return GetCurrWeapon()->GetArmsLeftHandTransform();
}

void UCWeaponComponent::Reload()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->Reload();
}

void UCWeaponComponent::Eject_Magazine()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->Eject_Magazine();
}

void UCWeaponComponent::Spawn_Magazine()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->Spawn_Magazine();
}

void UCWeaponComponent::Load_Magazine()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->Load_Magazine();
}

void UCWeaponComponent::End_Reload()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->End_Reload();
}

ACWeapon_Range* UCWeaponComponent::GetCurrWeapon()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckNullResult(Weapons[(int32)Type], nullptr);

	return Weapons[(int32)Type];
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode();
	
		return;
	}
	else if (IsUnarmedMode() == false)
	{
		CheckFalse(GetCurrWeapon()->CanUnequip());
	
		GetCurrWeapon()->Unequip();
	}
	
	CheckNull(Weapons[(int32)InType]);
	CheckFalse(Weapons[(int32)InType]->CanEquip());
	
	Weapons[(int32)InType]->Equip();
	
	ChangeType(InType);

	// if (!!HUD) HUD->SetVisibility(ESlateVisibility::Visible);
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType type = Type;
	Type = InType;

	if (OnWeaponTypeChanged.IsBound()) OnWeaponTypeChanged.Broadcast(type, InType);
}

void UCWeaponComponent::On_Begin_Aim(ACWeapon_Range* InThisWeapon)
{
	for (ACEquipment_Weapon* weapon : Weapons)
	{
		if (weapon == InThisWeapon)
			continue;

		weapon->SetHidden(true);
	}
}

void UCWeaponComponent::On_Begin_End()
{
	for (ACEquipment_Weapon* weapon : Weapons)
		weapon->SetHidden(false);
}

UCWeaponComponent::UCWeaponComponent()
{
	// CHelpers::GetClass<UCUserWidget_HUD>(&HUDClass, "WidgetBlueprint'/Game/Widgets/WB_HUD.WB_HUD_C'");
}

void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if (!!HUD)
	// {
	// 	if (!!GetCurrWeapon())
	// 		GetCurrWeapon()->IsAutoFire() ? HUD->OnAutoFire() : HUD->OffAutoFire();
	// 	else
	// 		HUD->OffAutoFire();
	//
	// 	if (!!GetCurrWeapon())
	// 	{
	// 		uint8 currCount = GetCurrWeapon()->GetCurrMagazineCount();
	// 		uint8 maxCount = GetCurrWeapon()->GetMaxMagazineCount();
	//
	// 		HUD->UpdateMagazine(currCount, maxCount);
	//
	// 		HUD->UpdateWeaponType(Type);
	// 	}
	// }
}

void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACNox_Runner>(GetOwner());
	CheckNull(OwnerCharacter);

	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (TSubclassOf<ACWeapon_Range> weaponClass : WeaponClasses)
	{
		if (!!weaponClass)
		{
			ACWeapon_Range* weapon = OwnerCharacter->GetWorld()->SpawnActor<ACWeapon_Range>(weaponClass, params);
			Weapons.Add(weapon);
		}
	}

	// if (!!HUDClass)
	// {
	// 	HUD = CreateWidget<UCUserWidget_HUD, APlayerController>(Owner->GetController<APlayerController>(), HUDClass);
	// 	HUD->AddToViewport();
	// 	HUD->SetVisibility(ESlateVisibility::Hidden);
	// }
	//
	// OnWeaponAim_Arms_Begin.AddDynamic(this, &UCWeaponComponent::On_Begin_Aim);
	// OnWeaponAim_Arms_End.AddDynamic(this, &UCWeaponComponent::On_Begin_End);
}
