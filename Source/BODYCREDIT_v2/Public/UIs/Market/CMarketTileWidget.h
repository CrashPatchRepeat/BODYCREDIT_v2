// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Items/Market/Base/CMarketStruct.h"
#include "CMarketTileWidget.generated.h"

class UCInventoryBaseComponent;
class UCMarketWidget;
class UImage;

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API UCMarketTileWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UCMarketWidget> OwningMarket;
	
	void SetItemImage(FSlateBrush NewBrush, FItemData InItemData, TObjectPtr<UCInventoryBaseComponent> InInventoryComponent);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Item;

	FItemData ItemData;

	UPROPERTY()
	TObjectPtr<UCInventoryBaseComponent> InventoryBaseComponent;
};
