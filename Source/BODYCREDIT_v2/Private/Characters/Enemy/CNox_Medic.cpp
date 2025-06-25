#include "Characters/Enemy/CNox_Medic.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/Enemy/AttackActor/CElectricGrenade.h"
#include "Components/CapsuleComponent.h"
#include "Components/Enemy/CNox_EAnimInstance.h"
#include "Components/Enemy/CNox_EStatusComp.h"
#include "Global.h"
#include "Characters/Enemy/State/CEnemyState.h"
#include "Components/BoxComponent.h"
#include "Components/Enemy/CNox_FSMComp.h"
#include "Engine/DamageEvents.h"

#pragma region 생성 및 초기화
ACNox_Medic::ACNox_Medic()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tmpMesh(TEXT(
		"/Game/Assets/Sci_Fi_Characters_Pack/Mesh/Sci_Fi_Character_02/SK_Sci_Fi_Character_02_Full.SK_Sci_Fi_Character_02_Full"));
	if (tmpMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(tmpMesh.Object);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -110), FRotator(0, -90, 0));
	GetMesh()->SetRelativeScale3D(FVector(1.55, 1.35, 1.3));

	GetCapsuleComponent()->SetCapsuleHalfHeight(110.f);
	GetCapsuleComponent()->SetCapsuleRadius(42.f);

	{
		// Attack Collision
		CHelpers::CreateComponent<UBoxComponent>(this, &AttackComp_l, "AttackComp_l", GetMesh(), "middle_01_l");
		CHelpers::CreateComponent<UBoxComponent>(this, &AttackComp_r, "AttackComp_r", GetMesh(), "middle_01_r");
		AttackComp_l->SetCollisionProfileName("EnemyWeapon");
		AttackComp_r->SetCollisionProfileName("EnemyWeapon");
		AttackComp_l->SetBoxExtent(FVector(25));
		AttackComp_r->SetBoxExtent(FVector(25));
		AttackComp_l->OnComponentBeginOverlap.AddDynamic(this, &ACNox_Medic::OnAttackComponentBeginOverlap);
		AttackComp_r->OnComponentBeginOverlap.AddDynamic(this, &ACNox_Medic::OnAttackComponentBeginOverlap);
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClass(
		TEXT("/Game/Characters/Enemy/Anim/MedicAnim/ABP_Medic.ABP_Medic_C"));
	if (AnimInstanceClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimInstanceClass.Class);

	EnemyType = EEnemyType::MedicAndroid;
	SetPerceptionInfo();

	ConstructorHelpers::FClassFinder<ACElectricGrenade> GrenadeClass(
		TEXT("/Game/Characters/Enemy/AttackActor/BP_ElectircGrenade.BP_ElectircGrenade_C"));
	if (GrenadeClass.Succeeded())
		ElectricGrenadeCls = GrenadeClass.Class;
}

void ACNox_Medic::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	ElectricGrenade = GetWorld()->SpawnActor<ACElectricGrenade>(ElectricGrenadeCls, this->GetActorLocation(),
																this->GetActorRotation(),
																SpawnParams);

	HealEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(HealEffectFactory, GetMesh(), "spine_02",
															  GetActorLocation(),
															  FRotator::ZeroRotator,
															  EAttachLocation::Type::KeepWorldPosition, false, false);

	AttackCollision(false); // Attack Collision Off

	StatusComp->SetMaxHealth(300);
}

void ACNox_Medic::SetPerceptionInfo()
{
	Super::SetPerceptionInfo();
	RetentionTime = 0.f;
}
#pragma endregion

#pragma region Take Damage
float ACNox_Medic::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (!GetTarget())
		if (ACNox* player = Cast<ACNox>(DamageCauser->GetOwner())) SetTarget(player);

	if (IsHealing()) return 0.f;

	StatusComp->TakeDamage(DamageAmount);
	if (StatusComp->IsDead())
	{
		AttackComp_l->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackComp_r->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (DamageCauser)
		{
			FVector ImpulseDir = GetActorLocation() - DamageCauser->GetActorLocation();
			ImpulseDir.Z = 0;
			ImpulseDir.Normalize();
			FVector Impulse = ImpulseDir * 5000.f;
			SetLastHitImpulse(Impulse);
		}
		if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
		{
			const FPointDamageEvent* PointEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
			SetLastHitInfo(PointEvent->HitInfo.ImpactPoint, PointEvent->HitInfo.BoneName);
		}
		else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
		{
			const FRadialDamageEvent* RadialEvent = static_cast<const FRadialDamageEvent*>(&DamageEvent);
			if (RadialEvent->ComponentHits.Num() > 0)
				SetLastHitInfo(RadialEvent->ComponentHits[0].ImpactPoint, RadialEvent->ComponentHits[0].BoneName);
		}
		else
		{
			SetLastHitInfo(GetActorLocation());
		}
		FSMComp->SetEnemyState(EEnemyState::Die);
	}
	else
	{
		if (FSMComp->GetEnemyState() == EEnemyState::Combat) return DamageAmount;

		PlayLaunchCharacter(1000);
		PlayHitStop(0.05);
		
		const float HitChance = 0.3f; // 30% 확률로 피격 상태 진입
		const float rand = FMath::FRand(); // 0~1 랜덤
		if (rand <= HitChance)
		{
			ResetVal();
			FSMComp->SetEnemyState(EEnemyState::Hit);
		}
	}
	return DamageAmount;
}
#pragma endregion

#pragma region Get New Speed
void ACNox_Medic::GetNewMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed, float& OutNewSpeed,
                                      float& OutNewAccelSpeed)
{
	switch (InMovementSpeed)
	{
	case EEnemyMovementSpeed::Idle:
		OutNewSpeed = 0.f;
		OutNewAccelSpeed = 0.f;
		break;
	case EEnemyMovementSpeed::Walking:
		OutNewSpeed = 280.f;
		OutNewAccelSpeed = 450.f;
		break;
	case EEnemyMovementSpeed::Sprinting:
		OutNewSpeed = 320.f;
		OutNewAccelSpeed = 1024.f;
		break;
	default:
		break;
	}
}
#pragma endregion

#pragma region Attack
void ACNox_Medic::OnAttackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ACNox_Medic::AttackCollision(bool bOn, bool IsRightHand)
{
	if (bOn)
	{
		if (IsRightHand)
		{
			AttackComp_r->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			AttackComp_l->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else
		{
			AttackComp_l->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			AttackComp_r->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	else
	{
		AttackComp_l->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackComp_r->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
#pragma endregion

#pragma region Grenade
void ACNox_Medic::HandleElectricGrenade()
{
	EnemyAnim->PlayGrenadeMontage();
}

bool ACNox_Medic::IsPlayingGrenade() const
{
	return EnemyAnim->IsPlayingGrenade();
}

void ACNox_Medic::LaunchElectricGrenade()
{
	FVector startLoc = GetMesh()->GetSocketLocation(FName("GrenadeSocket"));
	FVector targetLoc = Target->GetActorLocation();
	FVector outVelocity;
	SuggestProjectileVelocityWithLimit(outVelocity, this->GetActorLocation(), targetLoc);
	if (ElectricGrenade) ElectricGrenade->InitializeGrenade(startLoc, targetLoc, outVelocity);
}
#pragma endregion

#pragma region Heal
bool ACNox_Medic::IsLowHealth()
{
	return StatusComp->GetHealthPercent() <= HealStdValue;
}

void ACNox_Medic::HandleHeal(const bool bInEquipShield)
{
	EnemyAnim->PlayHealMontage(bInEquipShield);
	bInEquipShield ? HealEffect->Activate() : HealEffect->Deactivate();
}

bool ACNox_Medic::IsHealing() const
{
	return EnemyAnim->IsHealing();
}

void ACNox_Medic::SuggestProjectileVelocityWithLimit(FVector& OutVelocity, const FVector& StartLocation,
                                                     const FVector& TargetLocation, float MaxSpeed, float GravityZ)
{
	const FVector Delta = TargetLocation - StartLocation;
	FVector DeltaXY = FVector(Delta.X, Delta.Y, 0.f);
	float HorizontalDistance = DeltaXY.Size();
	float DeltaZ = Delta.Z;

	float ArcHeight = FMath::Clamp(HorizontalDistance * .2f, 100.f, 300.f);

	float Vz = FMath::Sqrt(2 * FMath::Abs(GravityZ) * ArcHeight);

	float TimeUp = Vz / FMath::Abs(GravityZ);
	float TimeDown = FMath::Sqrt(FMath::Max(2 * (ArcHeight - DeltaZ), 0.f) / FMath::Abs(GravityZ));
	float TotalTime = TimeUp + TimeDown;

	// 최소 시간 보정 (너무 가까울 때 처리)
	if (TotalTime <= 0.f) TotalTime = 0.1f;

	FVector DirXY = DeltaXY.GetSafeNormal();
	float Vxy = HorizontalDistance / TotalTime;

	FVector Velocity = DirXY * Vxy + FVector(0, 0, Vz);
	float InitialSpeed = Velocity.Size();

	if (InitialSpeed > MaxSpeed)
	{
		float Scale = MaxSpeed / InitialSpeed;
		Velocity *= Scale;
	}

	OutVelocity = Velocity;
}
#pragma endregion
