#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_CrossHair.generated.h"

class UBorder;

enum class EDirection
{
	Top, Bottom, Left, Right, Max,
};

UCLASS()
class BODYCREDIT_V2_API UCUserWidget_CrossHair : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateSpreadRange(float InRadius, float InMaxRadius);
	
protected:
	void NativeOnInitialized() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	TArray<UBorder*> Borders;
	TArray<FVector2D> Alignments;

	float Radius;
	float MaxRadius;
};
