#include "Items/Equipments/Weapons/Ranges/CWeapon_Range.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Characters/Runner/CNox_Runner.h"
#include "Widgets/Runners/CUserWidget_CrossHair.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/PlayerController.h"
#include "Items/Equipments/Weapons/Ranges/CMagazine.h"
#include "Items/Equipments/Weapons/Ranges/CBullet.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Components/DecalComponent.h"
#include "Characters/CNox.h"

void FWeaponAimData::SetData(ACNox* InOwner)
{
	USpringArmComponent* springArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	CheckNull(springArm);
	
	springArm->TargetArmLength = TargetArmLength;
	springArm->SocketOffset = SocketOffset;
	springArm->bEnableCameraLag = bEnableCameraLag;
}

void FWeaponAimData::SetDataByNoneCurve(ACNox* InOwner)
{
	USpringArmComponent* springArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	CheckNull(springArm);
	
	springArm->TargetArmLength = TargetArmLength;
	springArm->SocketOffset = SocketOffset;
	springArm->bEnableCameraLag = bEnableCameraLag;

	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(InOwner);
	CheckNull(camera);
	
	camera->FieldOfView = FieldOfView;
}

bool ACWeapon_Range::CanEquip()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;

	return !b;
}

void ACWeapon_Range::Equip()
{
	bEquipping = true;

	if (EquipMontage)
		OwnerCharacter->PlayAnimMontage(EquipMontage, EquipMontage_PlayRate);
}

void ACWeapon_Range::Begin_Equip()
{
	if (RightHandSocketName.IsValid())
		CHelpers::AttachTo(this, OwnerCharacter->GetMesh(), RightHandSocketName);
}

void ACWeapon_Range::End_Equip()
{
	bEquipping = false;

	if (CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Visible);
}

bool ACWeapon_Range::CanUnequip()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;

	return !b;
}

void ACWeapon_Range::Unequip()
{
	End_Aim();

	if (HolsterSocketName.IsValid())
		CHelpers::AttachTo(this, OwnerCharacter->GetMesh(), HolsterSocketName);

	if (CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Hidden);
}

bool ACWeapon_Range::CanAim()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bInAim;

	return !b;
}

void ACWeapon_Range::Begin_Aim()
{
	bInAim = true;

	if (!!AimCurve)
	{
		Timeline->PlayFromStart();
		AimData.SetData(OwnerCharacter);

		return;
	}

	AimData.SetDataByNoneCurve(OwnerCharacter);
}

void ACWeapon_Range::End_Aim()
{
	CheckFalse(bInAim);
	bInAim = false;

	if (!!AimCurve)
	{
		Timeline->ReverseFromEnd();
		BaseData.SetData(OwnerCharacter);

		return;
	}

	BaseData.SetDataByNoneCurve(OwnerCharacter);
}

bool ACWeapon_Range::CanFire()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;

	return !b;
}

void ACWeapon_Range::Begin_Fire()
{
	bFiring = true;

	if (bAutoFire)
	{
		GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &ACWeapon_Range::OnFiring, AutoFireInterval, true, 0);
		
		return;
	}

	OnFiring();
}

void ACWeapon_Range::End_Fire()
{
	CheckFalse(bFiring);

	if (GetWorld()->GetTimerManager().IsTimerActive(AutoFireHandle))
		GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle);

	bFiring = false;
}

void ACWeapon_Range::ToggleAutoFire()
{
	bAutoFire = !bAutoFire;
}

bool ACWeapon_Range::CanReload()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;

	return !b;
}

void ACWeapon_Range::Reload()
{
	CheckTrue(CurrMagazineCount == MaxMagazineCount);
	
	bReload = true;

	End_Aim();
	End_Fire();

	if (!!ReloadMontage)
		OwnerCharacter->PlayAnimMontage(ReloadMontage, ReloadMontage_PlayRate);
}

void ACWeapon_Range::Eject_Magazine()
{
	if (MagazineBoneName.IsValid())
		Mesh->HideBoneByName(MagazineBoneName, EPhysBodyOp::PBO_None);
	
	CheckNull(MagazineClass);
	
	FTransform transform = Mesh->GetSocketTransform(MagazineBoneName);
	ACMagazine* magazine = GetWorld()->SpawnActorDeferred<ACMagazine>(MagazineClass, transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	magazine->SetEject();
	magazine->SetLifeSpan(5);
	magazine->FinishSpawning(transform);
}

void ACWeapon_Range::Spawn_Magazine()
{
	CheckNull(MagazineClass);
	
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	Magazine = GetWorld()->SpawnActor<ACMagazine>(MagazineClass, params);
	CHelpers::AttachTo(Magazine, OwnerCharacter->GetMesh(), MagazineSocketName);
}

void ACWeapon_Range::Load_Magazine()
{
	CurrMagazineCount = MaxMagazineCount;
	if (MagazineBoneName.IsValid()) Mesh->UnHideBoneByName(MagazineBoneName);
	if (Magazine) Magazine->Destroy();
	SyncSightAmmoCount();
}

void ACWeapon_Range::End_Reload()
{
	bReload = false;
}

void ACWeapon_Range::SyncSightAmmoCount() {}

ACWeapon_Range::ACWeapon_Range()
{
	CHelpers::CreateActorComponent<UTimelineComponent>(this, &Timeline, "TimelineComponent");
	
	CHelpers::GetAsset<UCurveFloat>(&AimCurve, "/Script/Engine.CurveFloat'/Game/Items/Equipments/Weapons/Ranges/ARs/Curve_Aim.Curve_Aim'");
	
	CHelpers::GetAsset<UParticleSystem>(&HitParticle, "/Script/Engine.ParticleSystem'/Game/Items/Equipments/Weapons/Ranges/Effects/P_Impact_Default.P_Impact_Default'");

	CHelpers::GetAsset<UParticleSystem>(&FlashParticle, "/Script/Engine.ParticleSystem'/Game/Items/Equipments/Weapons/Ranges/Effects/P_Muzzleflash.P_Muzzleflash'");
	CHelpers::GetAsset<UParticleSystem>(&EjectParticle, "/Script/Engine.ParticleSystem'/Game/Items/Equipments/Weapons/Ranges/Effects/P_Eject_bullet.P_Eject_bullet'");
	CHelpers::GetClass<ACBullet>(&BulletClass, "/Script/Engine.Blueprint'/Game/Items/Equipments/Weapons/Ranges/Bullets/BP_CBullet.BP_CBullet_C'");

	CHelpers::GetAsset<UMaterialInstanceConstant>(&HitDecal, "/Script/Engine.MaterialInstanceConstant'/Game/Items/Equipments/Weapons/Ranges/Effects/Materials/MI_Hit.MI_Hit'");

}

void ACWeapon_Range::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LastAddSpreadTime >= 0.0f)
	{
		if (GetWorld()->GetTimeSeconds() - LastAddSpreadTime >= AutoFireInterval + 0.25f)
		{
			CurrSpreadRadius = 0.0f;
			LastAddSpreadTime = 0.0f;

			if(!!CrossHair)
				CrossHair->UpdateSpreadRange(CurrSpreadRadius, MaxSpreadAlignment);
		} //if
	}
}

void ACWeapon_Range::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACNox_Runner>(GetOwner());
	CheckNull(OwnerCharacter);

	if (HolsterSocketName.IsValid())
		CHelpers::AttachTo(this, OwnerCharacter->GetMesh(), HolsterSocketName);
	
	BaseData.SetDataByNoneCurve(OwnerCharacter);

	if (AimCurve)
	{
		FOnTimelineFloat timeline;
		timeline.BindUFunction(this, "OnAiming");

		Timeline->AddInterpFloat(AimCurve, timeline);
		Timeline->SetLooping(false);
		Timeline->SetPlayRate(AimingSpeed);
	}

	if (!!CrossHairClass)
	{
		CrossHair = CreateWidget<UCUserWidget_CrossHair, APlayerController*>(OwnerCharacter->GetController<APlayerController>(), CrossHairClass);
		CrossHair->AddToViewport();
		CrossHair->SetVisibility(ESlateVisibility::Hidden);
		CrossHair->UpdateSpreadRange(CurrSpreadRadius, MaxSpreadAlignment);
	}
	
	CurrMagazineCount = MaxMagazineCount;
}

void ACWeapon_Range::OnAiming(float Output)
{
	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	CheckNull(camera);
	
	camera->FieldOfView = FMath::Lerp(AimData.FieldOfView, BaseData.FieldOfView, Output);
}

void ACWeapon_Range::OnFiring()
{
	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	CheckNull(camera);
	
	FVector direction = camera->GetForwardVector();
	FTransform transform = camera->GetComponentToWorld();

	FVector start = transform.GetLocation() + direction;

	direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(direction, RecoilAngle);
	FVector end = transform.GetLocation() + direction * HitDistance;

	//DrawDebugLine(GetWorld(), start, end, FColor::Red, true, 5);

	TArray<AActor*> ignores;
	ignores.Add(this);
	ignores.Add(this->OwnerCharacter);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1, false, ignores, EDrawDebugTrace::None, hitResult, true);

	if (hitResult.bBlockingHit)
	{
		ACNox* nox = Cast<ACNox>(hitResult.GetActor());

		if (!nox)
		{
			if (HitDecal)
			{
				FRotator rotator = hitResult.ImpactNormal.Rotation();
				UDecalComponent* decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), HitDecal, FVector(5), hitResult.Location, rotator, 10);
				decal->SetFadeScreenSize(0);
			}

			if (HitParticle)
			{
				FRotator rotator = UKismetMathLibrary::FindLookAtRotation(hitResult.Location, hitResult.TraceStart);

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, hitResult.Location, rotator);
			}
		}
	}

	if (FlashParticle) UGameplayStatics::SpawnEmitterAttached(FlashParticle, Mesh, "muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

	if (EjectParticle) UGameplayStatics::SpawnEmitterAttached(EjectParticle, Mesh, "bullet_eject", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

	FVector muzzleLocation = Mesh->GetSocketLocation("muzzle");

	if (FireSound) UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, muzzleLocation);

	if (CameraShakeClass)
	{
		APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	
		if (!!controller) controller->PlayerCameraManager->StartCameraShake(CameraShakeClass);
	}
	
	OwnerCharacter->AddControllerPitchInput(-RecoilRate * UKismetMathLibrary::RandomFloatInRange(0.8f, 1.2f));
	
	if (CurrSpreadRadius <= 1.0f)
	{
		CurrSpreadRadius += SpreadSpeed * GetWorld()->GetDeltaSeconds();
	
		if (!!CrossHair) CrossHair->UpdateSpreadRange(CurrSpreadRadius, MaxSpreadAlignment);
	}
	LastAddSpreadTime = GetWorld()->GetTimeSeconds();
	
	if (!!BulletClass)
	{
		FVector location = Mesh->GetSocketLocation("muzzle");
	
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
		ACBullet* bullet = GetWorld()->SpawnActor<ACBullet>(BulletClass, location, direction.Rotation(), params);
	
		if (!!bullet) bullet->Shoot(direction);
	}

	if (--CurrMagazineCount >= 1)
	{
		/*CurrMagazineCount;*/
		SyncSightAmmoCount();
	}
	else
	{
		if (CanReload()) Reload();
	}
}
