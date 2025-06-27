#include "UIs/Lobby/CMarketWidget.h"

#include "UIs/Common/Buttons/CLobbyButtonBase.h"
#include "Global.h"

void UCMarketWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCMarketWidget::OnSelectWeaponClicked()
{
	// 무기 버튼 클릭 시 처리
	CLog::Print("Select Weapon Clicked");
}

void UCMarketWidget::OnSelectHeadClicked()
{
	// 머리 버튼 클릭 시 처리
	CLog::Print("Select Head Clicked");
}

void UCMarketWidget::OnSelectBodyClicked()
{
	// 몸통 버튼 클릭 시 처리
	CLog::Print("Select Body Clicked");
}

void UCMarketWidget::OnSelectArmClicked()
{
	// 팔 버튼 클릭 시 처리
	CLog::Print("Select Arm Clicked");
}

void UCMarketWidget::OnSelectLegClicked()
{
	// 다리 버튼 클릭 시 처리
	CLog::Print("Select Leg Clicked");
}

void UCMarketWidget::OnSelectBackpackClicked()
{
	// 백팩 버튼 클릭 시 처리
	CLog::Print("Select Backpack Clicked");
}
