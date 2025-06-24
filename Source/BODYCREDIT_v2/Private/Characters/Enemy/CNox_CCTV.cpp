#include "Characters/Enemy/CNox_CCTV.h"

#include "Characters/Enemy/CNox_EController.h"
#include "Characters/Enemy/State/CEnemyState.h"
#include "Components/Enemy/CNox_EStatusComp.h"
#include "Components/Enemy/CNox_FSMComp.h"
#include "Engine/OverlapResult.h"

void ACNox_CCTV::RotateCCTV(float DeltaTime)
{
	// 회전을 멈추고, 일정 시간이 지나면 다시 회전하도록 한다
	if (bIsPaused)
	{
		PauseTimer += DeltaTime;
		if (PauseTimer >= PauseTimeAtEnds)
		{
			PauseTimer = 0.f;
			bIsPaused = false;
			bRotatingRight = !bRotatingRight;
		}
		return;
	}

	FRotator CurrentRotation = GetActorRotation();
	float TargetYaw = bRotatingRight
						  ? InitialRotation.Yaw + MaxYaw
						  : InitialRotation.Yaw + MinYaw;

	float Direction = bRotatingRight ? 1.f : -1.f;
	float YawDelta = RotationSpeed * DeltaTime * Direction;
	float NewYaw = CurrentRotation.Yaw + YawDelta;
	SumRotYaw += YawDelta;

	// 회전이 끝점에 도달했는지 확인
	if ((bRotatingRight && SumRotYaw >= TargetYaw) || (!bRotatingRight && SumRotYaw <= TargetYaw))
	{
		NewYaw = TargetYaw;
		bIsPaused = true;
	}

	FRotator NewRotation = CurrentRotation;
	NewRotation.Yaw = NewYaw;
	SetActorRotation(NewRotation);
}

void ACNox_CCTV::BeginPlay()
{
	Super::BeginPlay();
	InitialRotation = GetActorRotation();
	SumRotYaw = InitialRotation.Yaw;

	StatusComp->SetMaxHealth(30);
}

void ACNox_CCTV::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FSMComp->GetEnemyState() == EEnemyState::Hit || FSMComp->GetEnemyState() == EEnemyState::Sense)
		FSMComp->SetEnemyState(EEnemyState::IDLE);
}

void ACNox_CCTV::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (auto* con = Cast<ACNox_EController>(NewController))
		con->OnDetectPlayer.BindUObject(this, &ACNox_CCTV::BroadCastDetectPlayer);
}

float ACNox_CCTV::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (StatusComp->IsDead()) return DamageAmount;
	
	StatusComp->TakeDamage(DamageAmount);
	if (StatusComp->IsDead()) FSMComp->SetEnemyState(EEnemyState::Die);
	return DamageAmount;
}

void ACNox_CCTV::BroadCastDetectPlayer(ACNox* DetectPlayer)
{
	FVector Origin = GetActorLocation();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		Origin,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel3), // 폰만 검색
		FCollisionShape::MakeSphere(3000.f),
		QueryParams
	);

	if (bDebug) DrawDebugSphere(GetWorld(), Origin, 3000.f, 8, FColor::Red, false, 1.0f);

	if (bHit)
	{
		for (auto& Result : OverlapResults)
		{
			ACNox_EBase* Enemy = Cast<ACNox_EBase>(Result.GetActor());
			if (!Enemy) continue;

			float ZDiff = Enemy->GetActorLocation().Z - Origin.Z;
			if (FMath::Abs(ZDiff) <= 300.f && ZDiff <= 0.f)
			{
				// 플레이어 감지 정보 전달
				Enemy->SetTarget(DetectPlayer);
			}
		}
	}
}
