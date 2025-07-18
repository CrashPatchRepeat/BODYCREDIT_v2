﻿#pragma once

#include "NativeGameplayTags.h"

namespace WidgetGameplayTags
{
	// 우선순위가 높은게 아래로 가도록 작성
	// BODYCREDIT_V2_API : 필요한 경우 다른 모듈의 게임 플레이 태그에 액세스 할 수 있음
	// Tag : 나중에 액세스 할 수 있는 변수 이름

	// Widget Stack
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_GameMenu);
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Popup);
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_GameHud);
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Frontend);

	// Widget
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Frontend_MainMenuScreen);
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Frontend_Option);
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Frontend_GameReady);
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Frontend_Inventory);
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Frontend_Market);
	
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_GameHud_Hud);
	
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Popup_Buy);
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Popup_Sell);
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Popup_FullBag);
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Popup_EnoughMoney);
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Popup_Rooting);
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Popup_BagPop);
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Popup_ItemPop);
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Popup_Guide);
	
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_GameMenu_Complete);
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_GameMenu_Fail);
	BODYCREDIT_V2_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_GameMenu_Pause);
}
