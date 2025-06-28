#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CInventoryGrid.generated.h"

class ACNoxController;
class UCInventoryItemWidget;
class UCInventoryBaseComponent;
class UBorder;
class UCanvasPanel;
class UCGameInstance;
class UCItemObject;
struct FInventoryLineDir;

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API UCInventoryGrid : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void InitInventory(UCInventoryBaseComponent* InventoryComponent, float Inventory_TileSize);
	void SetGridId(const int32 InGridId) { GridID = InGridId; }
	void SetPC(ACNoxController* InPC);
		
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* Border_Grid;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* Canvas_Grid;
	
	UPROPERTY()
	TObjectPtr<UCInventoryBaseComponent> InventoryBaseComp;
	UPROPERTY()
	TObjectPtr<UCGameInstance> GI;

	int32 InventoryRows = 0;
	int32 InventoryColumns = 0;
	float TileSize = 0.f;
	
	UPROPERTY()
	TArray<FInventoryLineDir> Lines;

	UPROPERTY(EditAnywhere, Category = Widget)
	TSubclassOf<UUserWidget> InventoryItemWidget;
	UPROPERTY()
	TObjectPtr<UCInventoryItemWidget> InventoryItemUI;
	UPROPERTY()
	TObjectPtr<ACNoxController> PlayerController;

	UPROPERTY(EditAnywhere)
	int32 GridID;	

	bool CurrentlyHovered = false;

	void CreateLineSegment();
	
	UFUNCTION()
	void Refresh();

	UFUNCTION()
	void OnItemRemoved(UCItemObject* ItemObject);

	bool IsCurrentlyHovered() const {return CurrentlyHovered;}
};
