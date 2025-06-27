#pragma once

#include "CoreMinimal.h"
#include "CMarketStruct.h"
#include "CItemObject.generated.h"

class UCItemStrategy;
class UCInventoryBaseComponent;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 0))
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 8))
	int32 CurrentIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 1))
	FIntPoint Dimensions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 3))
	FIntPoint StartPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 4))
	TSubclassOf<ACItemBase> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 5))
	UMaterialInterface* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 6))
	UMaterialInterface* RotatedIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 7))
	EPlayerPart ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 2))
	bool bRotated;

	FInventoryItemData()
		: ID(0)
		  , CurrentIndex(0)
		  , Dimensions(FIntPoint())
		  , StartPosition(FIntPoint())
		  , ItemType(EPlayerPart::Basic)
		  , bRotated(false)
	{}
};

USTRUCT(BlueprintType)
struct FItemSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 0))
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 8))
	int32 CurrentIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 1))
	FIntPoint Dimensions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 3))
	FIntPoint StartPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 4))
	TSubclassOf<ACItemBase> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 5))
	UMaterialInterface* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 6))
	UMaterialInterface* RotatedIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 7))
	EPlayerPart ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 9))
	TArray<FInventoryItemData> ContainedItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayOrder = 2))
	bool bRotated;

	FItemSaveData()
		: ID(0)
		  , CurrentIndex(0)
		  , Dimensions(FIntPoint())
		  , StartPosition(FIntPoint())
		  , ItemType(EPlayerPart::Basic)
		  , bRotated(false)
	{}
};

UCLASS()
class BODYCREDIT_V2_API UCItemObject : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Dimensions;

	FIntPoint GetDimension();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* RotatedIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACItemBase> ItemClass;

	UPROPERTY()
	bool Rotated = false;
	
	UPROPERTY()
	TWeakObjectPtr<ACItemBase> ItemActorOwner;

	UMaterialInterface* GetIcon();

	UMaterialInterface* GetThumbnail();

	TSubclassOf<ACItemBase> GetItemClass();

	UPROPERTY()
	FIntPoint StartPosition;

	void Rotate();
	bool IsRotated();
	FIntPoint GetStartPosition();

	int32 CurrentIndex = 0;

	int32 ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerPart ItemType = EPlayerPart::Basic;

	FItemSaveData ExportData() const;

	void ImportData(const FItemSaveData& Data);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventoryItemData> ContainedItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy")
	UCItemStrategy* ItemStrategy;

	UFUNCTION(BlueprintCallable, Category="Item")
	void SetItemStrategy(UCItemStrategy* NewStrategy);

	UFUNCTION(BlueprintCallable, Category="Item")
	void UseItem();

	UPROPERTY()
	bool bIsUseFunction = false;

	UPROPERTY()
	bool bIsMenu = false;

	void SetIconImages();

	int32 GetSellPrice();

	void SetItemStat();
	
	UPROPERTY()
	UCInventoryBaseComponent* OwnerInventoryComp;
	
};
