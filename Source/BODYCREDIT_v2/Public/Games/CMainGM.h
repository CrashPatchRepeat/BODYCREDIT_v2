#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CMainGM.generated.h"

class ACNox_Memory;
class ACAreaTriggerBox;
enum class EMemoryTriggerType:uint8;

UCLASS()
class BODYCREDIT_V2_API ACMainGM : public AGameMode
{
	GENERATED_BODY()

public:
	ACMainGM();

#pragma region Memory
	UPROPERTY(BlueprintReadWrite)
	bool ExtractTimerTriggerStart = false;
	UPROPERTY(EditAnywhere)
	float RegisterPercent = 0.4f;
	void RegisterMemoryFromPlayer(class ACNox_Runner* Player, EMemoryTriggerType Trigger);
#pragma endregion

private:
	virtual void BeginPlay() override;
	
#pragma region Memory
	UPROPERTY()
	ACNox_Memory* MemoryCollectorAI;
	UPROPERTY()
	TArray<ACAreaTriggerBox*> ZoneVolumes;
	const FString VIPZoneID = "VIP";

	FName GetZoneID(class ACNox_Runner* Player);
	bool IsInVIPZone(const FName& ZoneID);
#pragma endregion
};
