#include "UIs/Common/Buttons/CLobbyButtonBase.h"
#include "CommonTextBlock.h"
#include "CommonLazyImage.h"

void UCLobbyButtonBase::SetButtonText(FText InText, FVector2D InSize)
{
	if (CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		CommonTextBlock_ButtonText->SetText(bUserUpperCaseForButtonText ? InText.ToUpper() : InText);
	}

	if (UCommonLazyImage_BG && InSize != FVector2D::ZeroVector)
	{
		FSlateBrush NewBrush = UCommonLazyImage_BG->GetBrush();
		NewBrush.SetImageSize(InSize);
		UCommonLazyImage_BG->SetBrush(NewBrush);
	}
	else
	{
		FSlateBrush NewBrush = UCommonLazyImage_BG->GetBrush();
		NewBrush.SetImageSize(FVector2D(300, 130));
		UCommonLazyImage_BG->SetBrush(NewBrush);
	}
}

void UCLobbyButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetButtonText(ButtonDisplayText, BG_Size);
}
