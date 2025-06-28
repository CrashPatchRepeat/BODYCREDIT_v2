// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Items/Market/Base/CMarketStruct.h"
#include "CMarketTileWidget.generated.h"

class UCommonTextBlock;
class UCInventoryBaseComponent;
class UCMarketWidget;
class UImage;
class UVerticalBox;

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

	FSlateBrush GetItemBrush() const;
	void SetItemImage(FSlateBrush NewBrush, FItemData InItemData, TObjectPtr<UCInventoryBaseComponent> InInventoryComponent);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Item;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Txt_Name;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Txt_Price;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_ItemData;

	FItemData ItemData;

	UPROPERTY()
	TObjectPtr<UCInventoryBaseComponent> InventoryBaseComponent;

	virtual void NativeConstruct() override;
	void SetItemDescription();
	
	FString FormatNumberWithCommas(int32 Number);
};
