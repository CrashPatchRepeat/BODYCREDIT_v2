#include "Characters/Enemy/CNox_Memory.h"

#include "Characters/Enemy/AttackActor/CBeam.h"
#include "Characters/Enemy/AttackActor/CRangeProjectile.h"
#include "Characters/Enemy/AttackActor/CWavePulse.h"
#include "Components/Enemy/CNox_EAnimInstance.h"
#include "Global.h"

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
