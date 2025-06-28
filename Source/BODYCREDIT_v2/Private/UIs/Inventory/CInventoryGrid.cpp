// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/Inventory/CInventoryGrid.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Inventory/CInventoryBaseComponent.h"
#include "Components/Inventory/CInventoryTile.h"
#include "Games/CGameInstance.h"
#include "Games/CNoxController.h"
#include "Items/Market/Base/CItemObject.h"
#include "UIs/Inventory/CInventoryItemWidget.h"
#include "UIs/Inventory/CInventoryLine.h"

void UCInventoryGrid::InitInventory(UCInventoryBaseComponent* InventoryComponent, float Inventory_TileSize)
{
	InventoryBaseComp = InventoryComponent;
	InventoryRows = InventoryBaseComp->Rows;
	InventoryColumns = InventoryBaseComp->Columns;
	TileSize = Inventory_TileSize;

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Border_Grid->Slot);
	CanvasSlot->SetSize(FVector2D(InventoryColumns * TileSize, InventoryRows * TileSize));

	CreateLineSegment();

	Refresh();

	if (!InventoryBaseComp->InventoryChanged.IsAlreadyBound(this, &UCInventoryGrid::Refresh))
		InventoryBaseComp->InventoryChanged.AddDynamic(this, &UCInventoryGrid::Refresh);

	GI = Cast<UCGameInstance>(GetGameInstance());
}

void UCInventoryGrid::SetPC(ACNoxController* InPC)
{
	PlayerController = InPC;
}

int32 UCInventoryGrid::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                          bParentEnabled);

	if (Lines.Num() > 0) 
	{

		FVector2D GridSize = FVector2D(InventoryColumns * TileSize, InventoryRows * TileSize);
		FVector2f GridSizeF = FVector2f(GridSize);

		FPaintGeometry GridGeom = AllottedGeometry.ToPaintGeometry(
			GridSizeF,
			FSlateLayoutTransform(FVector2f::ZeroVector)
		);

		for (const FInventoryLineDir& Line : Lines)
		{
			TArray<FVector2D> Points;
			Points.Add(Line.Start);
			Points.Add(Line.End);

			FSlateDrawElement::MakeLines(
				OutDrawElements,
				LayerId + 1,
				GridGeom,
				Points,
				ESlateDrawEffect::None,
				FLinearColor::White,
				true,
				.5f
			);
		}
	

		if (!IsCurrentlyHovered()) {
			return LayerId + 1;
		}

		if (UWidgetBlueprintLibrary::IsDragDropping() && DrawDropLocation)
		{
			UDragDropOperation* CurrentOp = UWidgetBlueprintLibrary::GetDragDroppingContent();
			UCItemObject* ItemObject = CurrentOp ? Cast<UCItemObject>(CurrentOp->Payload) : nullptr;

			if (!ItemObject) {
				return LayerId + 1;
			}

			FLinearColor PossibleTint = IsRoomAvailableForPayload(ItemObject) ? FLinearColor(0.f, 1.f, 0.f, 0.3f) : FLinearColor(1.f, 0.f, 0.f, 0.3f);

			FVector2D BoxPosition = FVector2D(DraggedItemTopLeftTile.X * TileSize, DraggedItemTopLeftTile.Y * TileSize);
			FIntPoint Dim = ItemObject->GetDimension();
			FVector2D BoxSize = FVector2D(Dim.X * TileSize, Dim.Y * TileSize);

			FPaintGeometry BoxGeom = AllottedGeometry.ToPaintGeometry(
				FVector2f(BoxSize),
				FSlateLayoutTransform(FVector2f(BoxPosition))
			);

			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId + 1,
				BoxGeom,
				FCoreStyle::Get().GetBrush("WhiteBrush"),
				ESlateDrawEffect::None,
				PossibleTint
			);
		}
	}
	return LayerId + 2;
}

bool UCInventoryGrid::IsRoomAvailableForPayload(UCItemObject* ItemObject) const
{
	if (!IsCurrentlyHovered()) return false;
	if (IsValid(ItemObject))
	{
		FInventoryTile TempTile;
		TempTile.X = DraggedItemTopLeftTile.X;
		TempTile.Y = DraggedItemTopLeftTile.Y;

		return InventoryBaseComp->IsRoomAvailable(ItemObject, InventoryBaseComp->TileToIndex(TempTile));
	}
	
	return false;
}

void UCInventoryGrid::CreateLineSegment()
{
	int32 LocalX = 0;
	int32 LocalY = 0;
	FInventoryLineDir TempLine;

	for (int32 i = 0; i <= InventoryColumns; ++i)
	{
		LocalX = TileSize * i;

		TempLine.Start.X = LocalX;
		TempLine.Start.Y = 0;
		TempLine.End.X = LocalX;
		TempLine.End.Y = InventoryRows * TileSize;

		Lines.Add(TempLine);
	}

	for (int32 i = 0; i <= InventoryRows; ++i)
	{
		LocalY = TileSize * i;

		TempLine.Start.X = 0;
		TempLine.Start.Y = LocalY;
		TempLine.End.X = InventoryColumns * TileSize;
		TempLine.End.Y = LocalY;

		Lines.Add(TempLine);
	}
}

void UCInventoryGrid::Refresh()
{
	Canvas_Grid->ClearChildren();

	TMap<UCItemObject*, FInventoryTile> AllItem = InventoryBaseComp->GetAllItems();

	for (auto& Item : AllItem)
	{
		if (InventoryItemWidget)
		{
			InventoryItemUI = CreateWidget<UCInventoryItemWidget>(Cast<ACNoxController>(GetOwningPlayerPawn()->GetController()), InventoryItemWidget);
			if (!InventoryItemUI->OnItemRemoved.IsAlreadyBound(this, &UCInventoryGrid::OnItemRemoved))
				InventoryItemUI->OnItemRemoved.AddDynamic(this, &UCInventoryGrid::OnItemRemoved);

			InventoryItemUI->TileSize = TileSize;
			InventoryItemUI->ItemObject = Item.Key;

			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Canvas_Grid->AddChild(InventoryItemUI));

			if (CanvasSlot)
			{
				CanvasSlot->SetAutoSize(true);
				CanvasSlot->SetZOrder(10);
				CanvasSlot->SetPosition(FVector2D(Item.Value.X * TileSize, Item.Value.Y * TileSize));
			}
		}
	}
}

void UCInventoryGrid::OnItemRemoved(UCItemObject* ItemObject)
{
	if (!IsCurrentlyHovered() || !ItemObject) return;
	InventoryBaseComp->RemoveItem(ItemObject);
}
