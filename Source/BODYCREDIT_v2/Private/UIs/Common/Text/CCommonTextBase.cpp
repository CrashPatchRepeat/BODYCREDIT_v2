#include "UIs/Common/Text/CCommonTextBase.h"

#include "CommonTextBlock.h"

void UCCommonTextBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetButtonText(ButtonDisplayText);
}

void UCCommonTextBase::SetButtonText(FText InText)
{
	if (CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		CommonTextBlock_ButtonText->SetText(bUserUpperCaseForButtonText ? InText.ToUpper() : InText);
	}
}
