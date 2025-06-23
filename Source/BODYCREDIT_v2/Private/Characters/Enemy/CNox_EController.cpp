#include "Characters/Enemy/CNox_EController.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/CNox.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionTypes.h"
#include "Global.h"

#pragma region 생성자 및 초기화
ACNox_EController::ACNox_EController()
{
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	// Sight
	CHelpers::CreateActorComponent<UAISenseConfig_Sight>(this, &Sight, "Sight");
	Perception->ConfigureSense(*Sight);
	// Hearing
	CHelpers::CreateActorComponent<UAISenseConfig_Hearing>(this, &Hearing, "Hearing");
	Perception->ConfigureSense(*Hearing);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());

	EnemyBase = nullptr;
	TargetPlayer = nullptr;
}

void ACNox_EController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	EnemyBase = Cast<ACNox_EBase>(InPawn);
	if (EnemyBase) SetGenericTeamId(EnemyBase->GetTeamID()); // TeamID 설정
	
	InitPerception();
}
#pragma endregion

#pragma region Tick
void ACNox_EController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateExpiredStimuli(DeltaSeconds);
}
#pragma endregion

#pragma region Perception 초기화
void ACNox_EController::InitPerception()
{
    if (!Perception || !EnemyBase) return;
	
    SetupSightConfig();
	SetupHearingConfig();

	Perception->RequestStimuliListenerUpdate();
	Perception->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ACNox_EController::OnAITargetPerceptionInfoUpdate);
}

void ACNox_EController::SetupSightConfig()
{
	Sight->SightRadius = EnemyBase->GetSightRadius();
	Sight->LoseSightRadius = EnemyBase->GetLoseSightRadius();
	Sight->PeripheralVisionAngleDegrees = EnemyBase->GetPeripheralVisionAngleDegrees();
	Sight->SetMaxAge(EnemyBase->GetRetentionTime());
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
}

void ACNox_EController::SetupHearingConfig()
{
	Hearing->HearingRange = EnemyBase->GetHearingRange();
	Hearing->SetMaxAge(EnemyBase->GetRetentionTime());
	Hearing->DetectionByAffiliation.bDetectEnemies = true;
	Hearing->DetectionByAffiliation.bDetectNeutrals = false;
	Hearing->DetectionByAffiliation.bDetectFriendlies = false;
}
#pragma endregion

#pragma region Perception 비활성화
void ACNox_EController::PerceptionDeactive()
{
	if (Perception)
	{
		StopMovement();
		Perception->OnTargetPerceptionInfoUpdated.Clear();
	}
}
#pragma endregion

#pragma region 감지 이벤트 처리
void ACNox_EController::OnAITargetPerceptionInfoUpdate(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	auto StimulusType = UpdateInfo.Stimulus.Tag;
	if (StimulusType.ToString().Equals("Hearing"))
	{
		// 청각
		EnemyBase->bHearingMovement = true;
		EnemyBase->HearingLoc = UpdateInfo.Stimulus.StimulusLocation;
		return;
	}
	
	// 시각
	if (UpdateInfo.Stimulus.WasSuccessfullySensed())
	{
		if (TargetPlayer) return;
		TargetPlayer = GetNearTargetPlayer();

		StopMovement();
		EnemyBase->SetTarget(TargetPlayer);

		// CCTV가 플레이어를 발견하면 주변의 Enemy에게 알린다
		if (TargetPlayer) OnDetectPlayer.ExecuteIfBound(TargetPlayer);
	}
}
#pragma endregion

#pragma region 플레이어 감지
ACNox* ACNox_EController::GetNearTargetPlayer()
{
	ACNox* NearTarget = nullptr;
	float MinDistance = FLT_MAX;
	FVector MyLoc = EnemyBase->GetActorLocation();

	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);
	for (auto& actor : actors)
	{
		if (auto nox = Cast<ACNox>(actor))
		{
			float tmpDist = FVector::Dist(MyLoc, actor->GetActorLocation());
			if (tmpDist < MinDistance)
			{
				MinDistance = tmpDist;
				NearTarget = nox;
			}
		}
	}
	return NearTarget;
}
#pragma endregion

#pragma region 타겟 상실 처리
void ACNox_EController::UpdateExpiredStimuli(float DeltaTime)
{
	if (!TargetPlayer) return;
	if (FVector::Dist(TargetPlayer->GetActorLocation(), EnemyBase->GetActorLocation()) < Sight->LoseSightRadius)
	{
		CurExpiredTime = 0.f;
		return;
	}
	if (EnemyBase->GetRetentionTime() == KINDA_SMALL_NUMBER) return;

	CurExpiredTime += DeltaTime;
	if (CurExpiredTime >= Sight->GetMaxAge())
	{
		TargetPlayer = nullptr;
		CurExpiredTime = 0.f;
		EnemyBase->SetTarget(TargetPlayer);
		StopMovement();
	}
}
#pragma endregion

