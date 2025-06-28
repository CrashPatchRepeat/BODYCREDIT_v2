#include "UIs/WidgetGameplayTags.h"

namespace WidgetGameplayTags
{
	// 우선순위가 높은게 아래로 가도록 작성

	// Widget Stack
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Frontend, "WidgetStack.Frontend");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameHud, "WidgetStack.GameHud");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Modal, "WidgetStack.Popup");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameMenu, "WidgetStack.GameMenu");

	// Widget
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Frontend_MainMenuScreen, "WidgetStack.Frontend.MainMenuScreen");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Frontend_Option, "WidgetStack.Frontend.Option");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Frontend_GameReady, "WidgetStack.Frontend.GameReady");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Frontend_Inventory, "WidgetStack.Frontend.Inventory");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Frontend_Market, "WidgetStack.Frontend.Market");
	
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameHud_Hud, "WidgetStack.GameHud.Hud");
	
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Popup_Buy, "WidgetStack.Popup.Buy");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Popup_Sell, "WidgetStack.Popup.Sell");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Popup_FullBag, "WidgetStack.Popup.FullBag");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Popup_EnoughMoney, "WidgetStack.Popup.EnoughMoney");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Popup_Rooting, "WidgetStack.Popup.Rooting");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Popup_BagPop, "WidgetStack.Popup.BagPop");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Popup_ItemPop, "WidgetStack.Popup.ItemPop");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Popup_Guide, "WidgetStack.Popup.Guide");
						   
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameMenu_Complete, "WidgetStack.GameMenu.Complete");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameMenu_Fail, "WidgetStack.GameMenu.Fail");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameMenu_Pause, "WidgetStack.GameMenu.Pause");
}