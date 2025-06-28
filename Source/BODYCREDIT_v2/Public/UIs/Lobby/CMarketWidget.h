// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIs/Common/CWidgetActivatableBase.h"
#include "CMarketWidget.generated.h"

class UCGameInstance;
class UImage;
class UCInventoryGrid;
class UCMarketComponent;
class UCLobbyButtonBase;
class UCommonButtonBase;
class UCInventoryComponent;
class UCommonUserWidget;
class UVerticalBox;
class ACGameState;
class UTextBlock;

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API UCMarketWidget : public UCWidgetActivatableBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* VerticalBox_MarketItem;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCInventoryGrid* InventoryGridWidget;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Image_SelectWeapon_Hovered;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Txt_PlayerGold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FVector2D ImageSize = FVector2D(200, 150);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCommonUserWidget> MarketItemWidget;
	
	UPROPERTY()
	TObjectPtr<UCMarketComponent> MarketComp;
	UPROPERTY()
	TObjectPtr<UCInventoryComponent> InventoryComp;

	UPROPERTY()
	TObjectPtr<UImage> PreviousImage;
	UPROPERTY()
	TObjectPtr<UCGameInstance> GI;
	UPROPERTY()
	TObjectPtr<ACGameState> GS;
	
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void OnSelectWeaponClicked();
	UFUNCTION(BlueprintCallable)
	void OnSelectHeadClicked();
	UFUNCTION(BlueprintCallable)
	void OnSelectBodyClicked();
	UFUNCTION(BlueprintCallable)
	void OnSelectArmClicked();
	UFUNCTION(BlueprintCallable)
	void OnSelectLegClicked();
	UFUNCTION(BlueprintCallable)
	void OnSelectBackpackClicked();

	UFUNCTION()
	void UpdatePlayerGoldText(int32 NewGold);

	UFUNCTION()
	void RemoveWidget();

	void TurnOnPreviousImage();
	void TurnOffPreviousImage();
};
