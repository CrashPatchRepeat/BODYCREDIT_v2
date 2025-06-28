#pragma once

#include "CoreMinimal.h"
#include "Components/CBaseComponent.h"
#include "CMarketComponent.generated.h"

class UCGameInstance;
struct FItemData;
enum class EPlayerPart : uint8;

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API UCMarketComponent : public UCBaseComponent
{
	GENERATED_BODY()

public:
	TArray<FItemData> GetMarketItems(EPlayerPart ItemType);

private:
	UPROPERTY()
	TObjectPtr<UCGameInstance> GI;

	virtual void BeginPlay() override;
};
