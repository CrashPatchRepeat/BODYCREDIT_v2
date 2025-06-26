#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "CDeveloperSettings.generated.h"

class UCWidgetActivatableBase;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "UI Settings"))
class BODYCREDIT_V2_API UCDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Widaget Reference", meta=(ForceInlineRow, Categories="WidgetStack"))
	TMap<FGameplayTag, TSoftClassPtr<UCWidgetActivatableBase>> WidgetMap;
};
