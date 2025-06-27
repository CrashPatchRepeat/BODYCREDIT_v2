#include "UIs/Common/CWidget_PrimaryLayout.h"
#include "UIs/WidgetDebugHelper.h"

UCommonActivatableWidgetContainerBase* UCWidget_PrimaryLayout::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
	checkf(RegisteredWidgetStackMap.Contains(InTag), TEXT("Can not find the widget stack by the tag %s"), *InTag.ToString());
	return RegisteredWidgetStackMap.FindRef(InTag);
}

void UCWidget_PrimaryLayout::RegisterWidgetStack(UPARAM(meta=(Categories="WidgetStack")) FGameplayTag InStackTag,
                                                 UCommonActivatableWidgetContainerBase* InStack)
{
	if (!IsDesignTime()) // 화면에 그리는 중인지 확인
	{
		// 등록된 태그가 아니라면
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			// 스택을 등록
			RegisteredWidgetStackMap.Add(InStackTag, InStack);
			// WidgetDebug::Print(TEXT("Widget Stack Registered under the tag") + InStackTag.ToString());
		}
	}
}
