#include "Characters/Enemy/CNox_Zero.h"

#include "Characters/Enemy/State/CEnemyState.h"
#include "Global.h"
#include "Characters/Runner/CNox_Runner.h"
#include "Components/BoxComponent.h"
#include "Components/Enemy/CNox_EStatusComp.h"
#include "Components/Enemy/CNox_FSMComp.h"
#include "Engine/DamageEvents.h"

ACNox_Zero::ACNox_Zero()
{
	EnemyType = EEnemyType::Zero;
	{
		// Attack Collision
		CHelpers::CreateComponent<UBoxComponent>(this, &AttackComp_l, "AttackComp_l", GetMesh(), "middle_01_l");
		CHelpers::CreateComponent<UBoxComponent>(this, &AttackComp_r, "AttackComp_r", GetMesh(), "middle_01_r");
		AttackComp_l->SetCollisionProfileName("EnemyWeapon");
		AttackComp_r->SetCollisionProfileName("EnemyWeapon");
		AttackComp_l->SetBoxExtent(FVector(25));
		AttackComp_r->SetBoxExtent(FVector(25));
		AttackComp_l->OnComponentBeginOverlap.AddDynamic(this, &ACNox_Zero::OnAttackComponentBeginOverlap);
		AttackComp_r->OnComponentBeginOverlap.AddDynamic(this, &ACNox_Zero::OnAttackComponentBeginOverlap);
	}
}

void ACNox_Zero::BeginPlay()
{
	Super::BeginPlay();
	AttackCollision(false);
	StatusComp->SetMaxHealth(150);
}

float ACNox_Zero::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (!GetTarget())
		if (ACNox* player = Cast<ACNox>(DamageCauser->GetOwner())) SetTarget(player);
	
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

void ACNox_Zero::OnAttackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACNox_Runner::StaticClass()))
	{
		SetApplyDamage(OtherActor, 20.f);
	}
}

void ACNox_Zero::GetNewMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed, float& OutNewSpeed,
	float& OutNewAccelSpeed)
{
	switch (InMovementSpeed)
	{
	case EEnemyMovementSpeed::Idle:
		OutNewSpeed = 0.f;
		OutNewAccelSpeed = 0.f;
		break;
	case EEnemyMovementSpeed::Walking:
		OutNewSpeed = 40.f;
		OutNewAccelSpeed = 128.f;
		break;
	case EEnemyMovementSpeed::Sprinting:
		OutNewSpeed = 310.f;
		OutNewAccelSpeed = 1024.f;
		break;
	case EEnemyMovementSpeed::Combat:
		OutNewSpeed = 310.f;
		OutNewAccelSpeed = 1024.f;
		break;
	}
}

void ACNox_Zero::AttackCollision(bool bOn, bool IsRightHand)
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