#include "UIs/Common/AsyncActions/CAsyncAction_PushSoftWidget.h"

#include "Games/CGameInstanceSubsystem.h"
#include "UIs/Common/CWidgetActivatableBase.h"

UCAsyncAction_PushSoftWidget* UCAsyncAction_PushSoftWidget::PushSoftWidget(const UObject* WorldContextObject,
                                                                           APlayerController* OwningPlayerController,
                                                                           TSoftClassPtr<UCWidgetActivatableBase>
                                                                           InSoftWidgetClass,
                                                                           UPARAM(meta=(Categories="WidgetStack"))
                                                                           FGameplayTag InWidgetStackTag,
                                                                           bool bFocusOnNewlyPushedWidget)
{
    checkf(!InSoftWidgetClass.IsNull(), TEXT("PushSoftWidgetToStack was passed a null soft widget class "));

    if (GEngine)
    {
        if (UWorld* World = GEngine->
            GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
        {
            UCAsyncAction_PushSoftWidget* Node = NewObject<UCAsyncAction_PushSoftWidget>();
            Node->CachedOwningWorld = World;
            Node->CachedOwningPC = OwningPlayerController;
            Node->CachedSoftWidgetClass = InSoftWidgetClass;
            Node->CachedWidetStackTag = InWidgetStackTag;
            Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;

            Node->RegisterWithGameInstance(World);
            return Node;
        }
    }

    return nullptr;
}

void UCAsyncAction_PushSoftWidget::Activate()
{
    UCGameInstanceSubsystem* GISubSys = UCGameInstanceSubsystem::Get(CachedOwningWorld.Get());
    GISubSys->PushSoftWidgetToStackAynsc(CachedWidetStackTag, CachedSoftWidgetClass,
                                         [this](EAsyncPushWidgetState InPushState,
                                                UCWidgetActivatableBase* PushedWidget)
                                         {
                                             switch (InPushState)
                                             {
                                             case EAsyncPushWidgetState::OnCreatedBeforePush:
                                                 PushedWidget->SetOwningPlayer(CachedOwningPC.Get());
                                                 OnWidgetCreatedBeforePush.Broadcast(PushedWidget);
                                                 break;
                                             case EAsyncPushWidgetState::AfterPush:
                                                 AfterPush.Broadcast(PushedWidget);
                                                 if (bCachedFocusOnNewlyPushedWidget)
                                                 {
                                                     if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
                                                     {
                                                         WidgetToFocus->SetFocus();
                                                     }
                                                 }
                                                 SetReadyToDestroy();
                                                 break;
                                             }
                                         });
}
