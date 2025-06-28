#include "UIs/Market/CMarketTileWidget.h"

#include "CommonTextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UIs/Common/Text/CCommonTextBase.h"

void UCMarketTileWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Txt_Name)
		Txt_Name->SetText(FText::FromName(ItemData.ItemName));
	if (Txt_Price)
	{
		FString FormattedPrice = FormatNumberWithCommas(ItemData.Price);
		Txt_Price->SetText(FText::FromString(FString::Printf(TEXT("%s G"), *FormattedPrice)));
	}
	// SetItemDescription();
}

FSlateBrush UCMarketTileWidget::GetItemBrush() const
{
	if (!Image_Item) return FSlateBrush();
	return Image_Item->GetBrush();
}

void UCMarketTileWidget::SetItemImage(FSlateBrush NewBrush, FItemData InItemData,
                                      TObjectPtr<UCInventoryBaseComponent> InInventoryComponent)
{
	if (!Image_Item) return;
	Image_Item->SetBrush(NewBrush);
	ItemData = InItemData;
	InventoryBaseComponent = InInventoryComponent;
}

FString UCMarketTileWidget::FormatNumberWithCommas(int32 Number)
{
	FString NumberString = FString::FromInt(Number);
	FString Result;
	
	for (int32 i = 0; i < NumberString.Len(); ++i)
	{
		if (i > 0 && (NumberString.Len() - i) % 3 == 0)
		{
			Result += TEXT(",");
		}
		Result += NumberString[i];
	}
	
	return Result;
}

void UCMarketTileWidget::SetItemDescription()
{
	if (!VerticalBox_ItemData) return;

	VerticalBox_ItemData->ClearChildren();

	const FItemStatIncrease& Stat = ItemData.StatIncrease;

	bool bHasStat = false;

	auto AddStatText = [&](const FString& StatName, float Value)
	{
		if (FMath::IsNearlyZero(Value)) return;

		bHasStat = true;

		UTextBlock* StatText = NewObject<UTextBlock>(this);

		FString Sign = Value > 0.f ? TEXT("+") : TEXT("");
		FString Content = FString::Printf(TEXT("%s%.1f %s"), *Sign, Value, *StatName);

		StatText->SetText(FText::FromString(Content));
		FSlateFontInfo font = StatText->GetFont();
		font.Size = 12;
		StatText->SetFont(font);
		VerticalBox_ItemData->AddChild(StatText);
	};

	AddStatText(TEXT("Health"), Stat.Health);
	AddStatText(TEXT("Strength"), Stat.Strength);
	AddStatText(TEXT("Stamina"), Stat.Stamina);
	AddStatText(TEXT("Move Speed"), Stat.MoveSpeed);
	AddStatText(TEXT("Armor"), Stat.Armor);
	AddStatText(TEXT("Accuracy"), Stat.Accuracy);
	AddStatText(TEXT("Carry Weight"), Stat.CarryWeight);
	AddStatText(TEXT("Humanity"), Stat.Humanity);

	// if (!bHasStat && VerticalBox_ItemNamePrice)
	// {
	// 	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(VerticalBox_ItemNamePrice->Slot))
	// 		CanvasSlot->SetPosition(FVector2D(125.f, 0));
	// }
}
