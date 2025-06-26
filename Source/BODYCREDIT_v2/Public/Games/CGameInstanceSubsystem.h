#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CGameInstanceSubsystem.generated.h"

struct FGameplayTag;
class UCWidget_PrimaryLayout;
class UCWidgetActivatableBase;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush
};

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API UCGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UCGameInstanceSubsystem* Get(const UObject* WorldContextObject);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UCWidget_PrimaryLayout* InCreatedWidget);

	void PushSoftWidgetToStackAynsc(const FGameplayTag& InWidgetStackTag, TSoftClassPtr<UCWidgetActivatableBase> InSoftWidgetClass,
		TFunction<void(EAsyncPushWidgetState, UCWidgetActivatableBase*)> AysncPushStateCallback);
	
private:
	UPROPERTY(Transient)
	UCWidget_PrimaryLayout* CreatedPrimaryLayout;
};
