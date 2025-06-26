#include "UIs/WidgetGameplayTags.h"

namespace WidgetGameplayTags
{
	// 우선순위가 높은게 아래로 가도록 작성

	// Widget Stack
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Modal, "WidgetStack.Modal");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameMenu, "WidgetStack.GameMenu");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameHud, "WidgetStack.GameHud");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Frontend, "WidgetStack.Frontend");

	// Widget
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_PressAnyKeyScreen, "WidgetStack.PressAnyKeyScreen");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_MainMenuScreen, "WidgetStack.MainMenuScreen");
}