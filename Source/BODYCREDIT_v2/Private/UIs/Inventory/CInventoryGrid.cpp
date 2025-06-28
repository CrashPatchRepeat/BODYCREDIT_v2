// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/Inventory/CInventoryGrid.h"

#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Inventory/CInventoryBaseComponent.h"
#include "Components/Inventory/CInventoryTile.h"
#include "Games/CGameInstance.h"
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
			InventoryItemUI = CreateWidget<UCInventoryItemWidget>(GetWorld(), InventoryItemWidget);
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
