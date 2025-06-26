#include "Games/CGameInstanceSubsystem.h"

#include "UIs/Common/CWidgetActivatableBase.h"
#include "Engine/AssetManager.h"
#include "UIs/WidgetDebugHelper.h"
#include "UIs/Common/CWidget_PrimaryLayout.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

UCGameInstanceSubsystem* UCGameInstanceSubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UCGameInstanceSubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

bool UCGameInstanceSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);
		return FoundClasses.IsEmpty();
	}
	return false;
}

void UCGameInstanceSubsystem::RegisterCreatedPrimaryLayoutWidget(UCWidget_PrimaryLayout* InCreatedWidget)
{
	check(InCreatedWidget);
	CreatedPrimaryLayout = InCreatedWidget;
	WidgetDebug::Print(TEXT("Primary layout widget stored"));
}

void UCGameInstanceSubsystem::PushSoftWidgetToStackAynsc(const FGameplayTag& InWidgetStackTag,
                                                         TSoftClassPtr<UCWidgetActivatableBase> InSoftWidgetClass,
                                                         TFunction<void(EAsyncPushWidgetState, UCWidgetActivatableBase*)>
                                                         AysncPushStateCallback)
{
	check(!InSoftWidgetClass.IsNull());
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		InSoftWidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[InSoftWidgetClass, this, InWidgetStackTag, AysncPushStateCallback]()
			{
				UClass* LoadedWidgetClass = InSoftWidgetClass.Get();
				check(LoadedWidgetClass&&CreatedPrimaryLayout);
				UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatedPrimaryLayout->FindWidgetStackByTag(
					InWidgetStackTag);
				UCWidgetActivatableBase* CreatedWidget = FoundWidgetStack->AddWidget<UCWidgetActivatableBase>(
					LoadedWidgetClass,
					[AysncPushStateCallback](UCWidgetActivatableBase& CreatedWidgetInstance)
					{
						AysncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
					}
				);

				AysncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
			}
		)
	);
}
