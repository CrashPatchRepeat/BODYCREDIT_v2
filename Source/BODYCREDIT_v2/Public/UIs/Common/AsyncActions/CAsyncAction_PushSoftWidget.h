#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CAsyncAction_PushSoftWidget.generated.h"

class UCWidgetActivatableBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate, UCWidgetActivatableBase*, PushedWidget);
/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API UCAsyncAction_PushSoftWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,
		meta=(WorldContext="WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly="true",
			DisplayName = "Push Soft Widget To Widget Stack"))
	static UCAsyncAction_PushSoftWidget* PushSoftWidget(const UObject* WorldContextObject,
	                                                    APlayerController* OwningPlayerController,
	                                                    TSoftClassPtr<UCWidgetActivatableBase> InSoftWidgetClass,
	                                                    UPARAM(meta=(Categories="WidgetStack")) FGameplayTag InWidgetStackTag,
	                                                    bool bFocusOnNewlyPushedWidget = true);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate OnWidgetCreatedBeforePush;

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate AfterPush;

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	TWeakObjectPtr<APlayerController> CachedOwningPC;
	TSoftClassPtr<UCWidgetActivatableBase> CachedSoftWidgetClass;
	FGameplayTag CachedWidetStackTag;
	bool bCachedFocusOnNewlyPushedWidget = false;
};
