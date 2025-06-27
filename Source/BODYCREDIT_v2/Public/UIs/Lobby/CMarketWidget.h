// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIs/Common/CWidgetActivatableBase.h"
#include "CMarketWidget.generated.h"

class UCLobbyButtonBase;
class UCommonButtonBase;

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API UCMarketWidget : public UCWidgetActivatableBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* VerticalBox_MarketItem;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MarketItemWidget;
	
	
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
};
