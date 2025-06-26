#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CFunctionLibrary.generated.h"

class UCWidgetActivatableBase;
/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API UCFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Function Library")
	static TSoftClassPtr<UCWidgetActivatableBase> GetSoftWidgetClassByTag(UPARAM(meta=(Categories="WidgetStack")) FGameplayTag InWidgetTag);
};
