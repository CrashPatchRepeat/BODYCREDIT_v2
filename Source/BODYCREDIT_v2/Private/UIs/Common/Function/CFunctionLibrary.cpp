#include "UIs/Common/Function/CFunctionLibrary.h"
#include "UIs/Common/Settings/CDeveloperSettings.h"

TSoftClassPtr<UCWidgetActivatableBase> UCFunctionLibrary::GetSoftWidgetClassByTag(UPARAM(meta=(Categories="WidgetStack")) FGameplayTag InWidgetTag)
{
	const UCDeveloperSettings* DeveloperSettings = GetDefault<UCDeveloperSettings>();
	checkf(DeveloperSettings->WidgetMap.Contains(InWidgetTag),
	       TEXT("Could not find the corresponding widget under the tag %s"), *InWidgetTag.ToString());
	return DeveloperSettings->WidgetMap.FindRef(InWidgetTag);
}
