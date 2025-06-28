#include "UIs/Market/CMarketTileWidget.h"

#include "Components/Image.h"

void UCMarketTileWidget::SetItemImage(FSlateBrush NewBrush, FItemData InItemData,
	TObjectPtr<UCInventoryBaseComponent> InInventoryComponent)
{
	if (!Image_Item) return;
	Image_Item->SetBrush(NewBrush);
	ItemData = InItemData;
	InventoryBaseComponent = InInventoryComponent;
}
