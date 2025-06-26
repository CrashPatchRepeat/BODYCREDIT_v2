// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CCommonTextBase.generated.h"

class UCommonTextBlock;

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API UCCommonTextBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText);

private:
	virtual void NativePreConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Button", meta=(AllowPrivateAccess="true"))
	FText ButtonDisplayText;
	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_ButtonText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Button", meta=(AllowPrivateAccess="true"))
	float FontSize = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Button", meta=(AllowPrivateAccess="true"))
	bool bUserUpperCaseForButtonText = false;
};
