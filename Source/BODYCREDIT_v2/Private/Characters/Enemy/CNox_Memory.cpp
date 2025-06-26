#include "Characters/Enemy/CNox_Memory.h"

#include "Characters/Enemy/AttackActor/CBeam.h"
#include "Characters/Enemy/AttackActor/CRangeProjectile.h"
#include "Characters/Enemy/AttackActor/CWavePulse.h"
#include "Components/Enemy/CNox_EAnimInstance.h"
#include "Global.h"
#include "Characters/Enemy/State/CEnemyState.h"
#include "Components/Enemy/CNox_EStatusComp.h"
#include "Components/Enemy/CNox_FSMComp.h"
#include "Engine/DamageEvents.h"
#include "Transportation/CVent.h"
#include "Transportation/CStair.h"

ACNox_Memory::ACNox_Memory()
{
	{
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -85), FRotator(0, -90, 0));
		GetMesh()->SetRelativeScale3D(FVector(1.1));

		GetCapsuleComponent()->SetCapsuleHalfHeight(100.f);
		GetCapsuleComponent()->SetCapsuleRadius(34.f);

		GetMesh()->SetRelativeLocation(FVector(0, 0, -100));
	}

	EnemyType = EEnemyType::MemoryCollector;
	SetPerceptionInfo();
}

void ACNox_Memory::BeginPlay()
{
	Super::BeginPlay();
	{
		CHelpers::FindActors<ACVent>(GetWorld(), AllVent);
		CHelpers::FindActors<ACStair>(GetWorld(), AllStair);
	}

	{
		// Beam
		FVector SpawnLocation = FVector::ZeroVector;
		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters params;
		params.Owner = this;
		Beam = GetWorld()->SpawnActor<ACBeam>(BeamOrgCls, SpawnLocation, SpawnRotation, params);
		if (Beam)
		{
			Beam->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("BeamSocket"));
			Beam->SetActorHiddenInGame(true);
		}
	}

	{
		// WavePulse
		FVector SpawnLocation = FVector::ZeroVector;
		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters params;
		params.Owner = this;
		WavePulse = GetWorld()->SpawnActor<ACWavePulse>(WavePulseOrgCls, SpawnLocation, SpawnRotation, params);
		if (WavePulse)
		{
			WavePulse->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
										 FName("PulseWaveSocket"));
			WavePulse->SetActorHiddenInGame(true);
		}
	}

	{
		// RangeProjectile
		SpawnRangeProjectile();
	}
}

void ACNox_Memory::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bRotateToTarget)
	{
		RotateToTarget(DeltaSeconds, GetTransform(), Target->GetActorLocation());
	}
}

void ACNox_Memory::SetPerceptionInfo()
{
	Super::SetPerceptionInfo();
	SightRadius = 800.f;
	HearingRange = 1000.f;

	RetentionTime = 0.f;
}

float ACNox_Memory::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (!GetTarget())
		if (ACNox* player = Cast<ACNox>(DamageCauser->GetOwner())) SetTarget(player);
	
	StatusComp->TakeDamage(DamageAmount);
	if (StatusComp->IsDead()) {
		if (DamageCauser)
		{
			FVector ImpulseDir = GetActorLocation() - DamageCauser->GetActorLocation();
			ImpulseDir.Z = 0;
			ImpulseDir.Normalize();
			FVector Impulse = ImpulseDir * 5000.f;
			SetLastHitImpulse(Impulse);
		}
		// 피격 위치/본 정보 저장
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

		PlayLaunchCharacter(500);
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

void ACNox_Memory::GetNewMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed, float& OutNewSpeed,
	float& OutNewAccelSpeed)
{
	switch (InMovementSpeed)
	{
	case EEnemyMovementSpeed::Idle:
		OutNewSpeed = 0.f;
		OutNewAccelSpeed = 0.f;
		break;
	case EEnemyMovementSpeed::Walking:
		OutNewSpeed = 400.f;
		OutNewAccelSpeed = 728.f;
		break;
	case EEnemyMovementSpeed::Sprinting:
		OutNewSpeed = 400.f;
		OutNewAccelSpeed = 1024.f;
		break;
	}
}

#pragma region Beam
void ACNox_Memory::ShutBeam()
{
	EnemyAnim->PlayBeamAttack();
}

bool ACNox_Memory::IsPlayBeam()
{
	return EnemyAnim->IsBeamAttacking();
}

void ACNox_Memory::BeamAttack()
{
	Beam->SetBeamActive(true, Target);
	bRotateToTarget = true;
}

void ACNox_Memory::BeamAttackEnd()
{
	EnemyAnim->StopBeamAttack();

	Beam->SetBeamActive(false, Target);
	bRotateToTarget = false;
}
#pragma endregion

#pragma region Wave Pulse
void ACNox_Memory::ShutWavePulse()
{
	EnemyAnim->PlayWavePulse();
}

bool ACNox_Memory::IsPlayWavePulse()
{
	return EnemyAnim->IsWavePulseAttacking();
}

void ACNox_Memory::WavePulseAttack()
{
	WavePulse->StartWave();
}
#pragma endregion

#pragma region Memory
void ACNox_Memory::RegisterMemory(const FMemoryFragment& InNewMemory)
{
	// 중복 방지: 동일 위치/플레이어 기억은 덮어쓰기 또는 무시
	for (FMemoryFragment& Mem : MemoryQueue)
	{
		if (Mem.TriggerType == InNewMemory.TriggerType &&
			Mem.ZoneID == InNewMemory.ZoneID)
		{
			if (InNewMemory.TimeStamp > Mem.TimeStamp)
			{
				Mem = InNewMemory; // 더 최신이면 갱신
			}
			return; // 중복 방지
		}
	}

	// 새로운 기억 추가
	MemoryQueue.Add(InNewMemory);

	// 우선순위 재계산
	FVector AILocation = GetActorLocation();
	for (FMemoryFragment& Mem : MemoryQueue)
	{
		Mem.CalculatePriority(AILocation);
	}

	MemoryQueue.Sort(); // operator< 로 우선순위 내림차순 정렬
}

bool ACNox_Memory::EvaluateMemory()
{
	float Now = GetWorld()->GetTimeSeconds();
	float BestScore = -1.0f;
	FMemoryFragment* BestMemory = nullptr;

	// TTL 검사: 유효하지 않은 기억 제거
	MemoryQueue.RemoveAll([Now, this](const FMemoryFragment& Mem)
	{
		return (Now - Mem.TimeStamp) > MemoryExpireTime;
	});

	for (FMemoryFragment& Mem : MemoryQueue)
	{
		float Score = 0.0f;

		// 기억의 종류에 따라 가중치 부여
		switch (Mem.TriggerType)
		{
		case EMemoryTriggerType::Stay:
			Score += 20.f;
			break;
		case EMemoryTriggerType::Combat:
			Score += 40.f;
			break;
		case EMemoryTriggerType::Looting:
			Score += 15.f;
			break;
		default:
			break;
		}

		// 거리 기반 가중치 (가까울수록 점수 증가)
		const float Distance = FVector::Dist(Mem.Location, GetActorLocation());
		Score += FMath::Clamp(1000.f - Distance, 0.f, 1000.f) * 0.01f; // 최대 +10

		// 최신성 가중치 (최근일수록 점수 증가)
		const float Age = Now - Mem.TimeStamp;
		Score += FMath::Clamp(10.f - Age, 0.f, 10.f); // 최대 +10

		// 최고 점수 기억 업데이트
		if (Score > BestScore)
		{
			BestScore = Score;
			BestMemory = &Mem;
		}
	}

	if (BestMemory) CurrentTargetMemory = *BestMemory;

	return BestMemory ? true : false;
}

void ACNox_Memory::SetMemoryTarget_MemoryMoveEnd(const FMemoryFragment& InNewMemory)
{
	if (MemoryQueue.Contains(CurrentTargetMemory))
		MemoryQueue.Remove(CurrentTargetMemory);
	CurrentTargetMemory = InNewMemory;
}
#pragma endregion

void ACNox_Memory::StartRangeAttack(bool bIsRight)
{
	if (RangeProjectileArray.Num() == 0) SpawnRangeProjectile();

	FTransform SpawnTransform = bIsRight
									? GetMesh()->GetSocketTransform(FName("RangeAttackSocket_r"), RTS_World)
									: GetMesh()->GetSocketTransform(FName("RangeAttackSocket_l"), RTS_World);
	SpawnTransform.SetScale3D(SpawnScale);

	auto* PoolObj = RangeProjectileArray.Pop();
	PoolObj->InitializeProjectile(SpawnTransform.GetLocation(), Target);
	PoolObj->SetActorTransform(SpawnTransform);
	PoolObj->SetActorHiddenInGame(false);
	PoolObj->SetCollisionEnabled(true);
	PoolObj->SetActorTickEnabled(true);
}

void ACNox_Memory::ReturnToPool(ACRangeProjectile* ReturnedProjectile)
{
	if (RangeProjectileArray.Num() == 0 || !RangeProjectileArray.Contains(ReturnedProjectile))
	{
		RangeProjectileArray.Emplace(ReturnedProjectile);
	}

	ReturnedProjectile->SetActorHiddenInGame(true);
	ReturnedProjectile->SetCollisionEnabled(false);
	ReturnedProjectile->SetActorTickEnabled(false);
}

void ACNox_Memory::SpawnRangeProjectile()
{
	for (int32 i = 0; i < SpawnProjectileCount; ++i)
	{
		FTransform Transform(FRotator::ZeroRotator, FVector::ZeroVector, SpawnScale);
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ACRangeProjectile* PoolObj = GetWorld()->SpawnActorDeferred<ACRangeProjectile>(
			RangeProjectileCls, Transform, this, nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		PoolObj->SetActorEnableCollision(false);
		PoolObj->SetActorHiddenInGame(true);
		PoolObj->SetActorTickEnabled(false);

		UGameplayStatics::FinishSpawningActor(PoolObj, Transform);
		RangeProjectileArray.Emplace(PoolObj);
	}
}
