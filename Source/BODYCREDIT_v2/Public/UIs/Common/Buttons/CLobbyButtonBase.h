#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CLobbyButtonBase.generated.h"

class UCommonTextBlock;
class UCommonLazyImage;

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API UCLobbyButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

private:
	virtual void NativePreConstruct() override;
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText, FVector2D InSize);

	UPROPERTY(meta = (BindWidgetOptional))
	UCommonLazyImage* UCommonLazyImage_BG;
	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Button", meta=(AllowPrivateAccess="true"))
	FText ButtonDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Button", meta=(AllowPrivateAccess="true"))
	bool bUserUpperCaseForButtonText = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Button", meta=(AllowPrivateAccess="true"))
	FVector2D BG_Size = FVector2D::ZeroVector;
};
