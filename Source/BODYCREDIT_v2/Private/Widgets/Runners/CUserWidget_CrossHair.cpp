#include "Widgets/Runners/CUserWidget_CrossHair.h"
#include "Global.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "blueprint/WidgetLayoutLibrary.h"

void UCUserWidget_CrossHair::UpdateSpreadRange(float InRadius, float InMaxRadius)
{
	Radius = InRadius;
	MaxRadius = InMaxRadius;
}

void UCUserWidget_CrossHair::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UCanvasPanel* panel = Cast<UCanvasPanel>(WidgetTree->RootWidget);
	CheckNull(panel);

	TArray<UWidget*> widgets = panel->GetAllChildren();
	for (UWidget* widget : widgets)
	{
		UBorder* border = Cast<UBorder>(widget);
		if (border == nullptr) continue;

		Borders.Add(border);

		UCanvasPanelSlot* slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(border);
		Alignments.Add(slot->GetAlignment());
	}
}

void UCUserWidget_CrossHair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float minimum = 0.0f, maximum = 0.0f;
	for (int32 i = 0; i < (int32)EDirection::Max; i++)
	{
		switch ((EDirection)i)
		{
			case EDirection::Top:
				{
					minimum = Alignments[i].Y;
					maximum = Alignments[i].Y + MaxRadius;
				}
				break;

			case EDirection::Bottom:
				{
					minimum = Alignments[i].Y;
					maximum = Alignments[i].Y - MaxRadius;
				}
				break;

			case EDirection::Left:
				{
					minimum = Alignments[i].X;
					maximum = Alignments[i].X + MaxRadius;
				}
				break;

			case EDirection::Right:
				{
					minimum = Alignments[i].X;
					maximum = Alignments[i].X - MaxRadius;
				}
				break;
		} //switch

		float value = FMath::Lerp<float>(minimum, maximum, Radius);

		UCanvasPanelSlot* slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Borders[i]);
		switch ((EDirection)i)
		{
			case EDirection::Top:
			case EDirection::Bottom:
				slot->SetAlignment(FVector2D(Alignments[i].X, value));
				break;

			case EDirection::Left:
			case EDirection::Right:
				slot->SetAlignment(FVector2D(value, Alignments[i].Y));
				break;
		} //switch
	}
}
