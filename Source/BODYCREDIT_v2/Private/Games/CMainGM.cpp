#include "Games/CMainGM.h"

#include "EngineUtils.h"
#include "Global.h"
#include "Characters/Enemy/CNox_Memory.h"
#include "Characters/Enemy/State/CMemoryData.h"
#include "Characters/Runner/CNox_Runner.h"
#include "Trigger/CAreaTriggerBox.h"

ACMainGM::ACMainGM()
{
	// DefaultPawnClass
	CHelpers::GetClass(&DefaultPawnClass, TEXT("/Script/Engine.Blueprint'/Game/Characters/Runner/BP_CNox_Runner.BP_CNox_Runner_C'"));

	// PlayerController
	CHelpers::GetClass(&PlayerControllerClass, TEXT("/Script/Engine.Blueprint'/Game/Games/BP_CNoxController.BP_CNoxController_C'"));
}

#pragma region App Version
const FString ACMainGM::GetAppVersion()
{
	FString AppVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		AppVersion,
		GGameIni
	);

	return AppVersion;
}
#pragma endregion

void ACMainGM::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ACNox_Memory> It(GetWorld(), ACNox_Memory::StaticClass()); It; ++It)
	{
		MemoryCollectorAI = *It;
	}

	for (TActorIterator<ACAreaTriggerBox> It(GetWorld(), ACAreaTriggerBox::StaticClass()); It; ++It)
	{
		ZoneVolumes.Add(*It);
	}
}

void ACMainGM::RegisterMemoryFromPlayer(class ACNox_Runner* Player, EMemoryTriggerType Trigger)
{
	if (!ExtractTimerTriggerStart && !Player) return;
	if (FMath::FRandRange(0.0, 1.0) > RegisterPercent) return;

	FMemoryFragment NewMemory;
	NewMemory.Location = Player->GetActorLocation();
	NewMemory.TimeStamp = GetWorld()->GetTimeSeconds();
	NewMemory.TriggerType = Trigger;
	NewMemory.SourcePlayer = Player;
	NewMemory.ZoneID = GetZoneID(Player);
	NewMemory.bVIPRelated = IsInVIPZone(NewMemory.ZoneID);

	if (!MemoryCollectorAI) return;
	MemoryCollectorAI->RegisterMemory(NewMemory); // AI에 직접 등록
}

FName ACMainGM::GetZoneID(class ACNox_Runner* Player)
{
	if (!Player) return NAME_None;

	for (const auto& Zone : ZoneVolumes)
	{
		if (Zone && Zone->IsOverlappingActor(Player))
		{
			return Zone->ZoneID;
		}
	}
	return NAME_None;
}

bool ACMainGM::IsInVIPZone(const FName& ZoneID)
{
	return ZoneID.ToString().Equals(VIPZoneID);
}