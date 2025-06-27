#include "Components/Runner/CWeaponComponent.h"
#include "Global.h"
#include "Characters/Runner/CNox_Runner.h"
#include "Widgets/Runners/CUserWidget_HUD.h"
#include "GameFramework/PlayerController.h"
#include "Items/Equipments/Weapons/CEquipment_Weapon.h"
#include "items/Equipments/Weapons/Ranges/CWeapon_Range.h"
#include "EnhancedInputComponent.h"

void UCWeaponComponent::SetupInputBindings(UEnhancedInputComponent* InEnhancedInputComponent)
{
	CheckNull(InEnhancedInputComponent);

	InEnhancedInputComponent->BindAction(IA_WeaponSlot1, ETriggerEvent::Started, this, &UCWeaponComponent::SetAKMode);
	InEnhancedInputComponent->BindAction(IA_WeaponSlot2, ETriggerEvent::Started, this, &UCWeaponComponent::SetM4Mode);

	InEnhancedInputComponent->BindAction(IA_Action, ETriggerEvent::Started, this, &UCWeaponComponent::Begin_Fire);
	InEnhancedInputComponent->BindAction(IA_Action, ETriggerEvent::Completed, this, &UCWeaponComponent::End_Fire);

	InEnhancedInputComponent->BindAction(IA_SubAction, ETriggerEvent::Started, this, &UCWeaponComponent::Begin_Aim);
	InEnhancedInputComponent->BindAction(IA_SubAction, ETriggerEvent::Completed, this, &UCWeaponComponent::End_Aim);

	InEnhancedInputComponent->BindAction(IA_AutoFire, ETriggerEvent::Started, this, &UCWeaponComponent::ToggleAutoFire);
	
	InEnhancedInputComponent->BindAction(IA_Reload, ETriggerEvent::Started, this, &UCWeaponComponent::Reload);
}

void UCWeaponComponent::SetUnarmedMode()
{
	CheckFalse(GetCurrWeapon()->CanUnequip());

	GetCurrWeapon()->Unequip();
	ChangeType(EWeaponType::Max);

	if (HUD) HUD->SetVisibility(ESlateVisibility::Hidden);
}

void UCWeaponComponent::SetAKMode()
{
	SetMode(EWeaponType::AK);
}

void UCWeaponComponent::SetM4Mode()
{
	SetMode(EWeaponType::M4);
}

void UCWeaponComponent::SetMagnumMode()
{
	SetMode(EWeaponType::Magnum);
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
	CheckFalse(GetCurrWeapon()->CanAim());

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
	CheckFalse(GetCurrWeapon()->CanReload());

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

	if (!!HUD) HUD->SetVisibility(ESlateVisibility::Visible);
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
		if (weapon == InThisWeapon) continue;

		weapon->SetHidden(true);
	}
}

void UCWeaponComponent::On_Begin_End()
{
	for (ACEquipment_Weapon* weapon : Weapons) weapon->SetHidden(false);
}

UCWeaponComponent::UCWeaponComponent()
{
	CHelpers::GetClass<UCUserWidget_HUD>(&HUDClass, "/Script/UMGEditor.WidgetBlueprint'/Game/UIs/Runner/WBP_CUserWidget_HUD.WBP_CUserWidget_HUD_C'");

	CHelpers::GetAsset(&IA_WeaponSlot1, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_WeaponSlot1.IA_WeaponSlot1'"));
	CHelpers::GetAsset(&IA_WeaponSlot2, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_WeaponSlot2.IA_WeaponSlot2'"));
	
	CHelpers::GetAsset(&IA_Action, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Action.IA_Action'"));
	CHelpers::GetAsset(&IA_SubAction, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_SubAction.IA_SubAction'"));
	
	CHelpers::GetAsset(&IA_AutoFire, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_AutoFire.IA_AutoFire'"));
	
	CHelpers::GetAsset(&IA_Reload, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Reload.IA_Reload'"));
}

void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if (GEngine)
	// {
	// 	// UEnum 객체 얻기 (패키지 경로는 ANY_PACKAGE)
	// 	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);
	// 	if (EnumPtr)
	// 	{
	// 		// enum 값을 문자열로 변환
	// 		FString WeaponName = EnumPtr->GetNameByValue((int64)Type).ToString();
	//
	// 		// 화면에 출력 (키 -1 은 자동으로 새로운 메시지 ID 부여)
	// 		GEngine->AddOnScreenDebugMessage(
	// 			-1,           // 메시지 ID
	// 			0.0f,         // 화면에 남기는 시간 (0이면 다음 Tick 때 지워짐)
	// 			FColor::White,// 텍스트 색
	// 			FString::Printf(TEXT("Weapon: %s"), *WeaponName)
	// 		);
	// 	}
	// }
	
	if (!!HUD)
	{
		if (!!GetCurrWeapon())
			GetCurrWeapon()->IsAutoFire() ? HUD->OnAutoFire() : HUD->OffAutoFire();
		else
			HUD->OffAutoFire();
	
		if (!!GetCurrWeapon())
		{
			uint8 currCount = GetCurrWeapon()->GetCurrMagazineCount();
			uint8 maxCount = GetCurrWeapon()->GetMaxMagazineCount();
	
			HUD->UpdateMagazine(currCount, maxCount);
	
			HUD->UpdateWeaponType(Type);
		}
	}
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

	if (!!HUDClass)
	{
		HUD = CreateWidget<UCUserWidget_HUD, APlayerController*>(OwnerCharacter->GetController<APlayerController>(), HUDClass);
		HUD->AddToViewport();
		HUD->SetVisibility(ESlateVisibility::Hidden);
	}
	
	OnWeaponAim_Arms_Begin.AddDynamic(this, &UCWeaponComponent::On_Begin_Aim);
	OnWeaponAim_Arms_End.AddDynamic(this, &UCWeaponComponent::On_Begin_End);
}
