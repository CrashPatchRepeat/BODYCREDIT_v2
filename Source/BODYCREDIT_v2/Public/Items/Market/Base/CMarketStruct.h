#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Components/Runner/CWeaponComponent.h"
#include "CMarketStruct.generated.h"

class ACItemBase;

USTRUCT(BlueprintType)
struct FItemRarityIcon
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UMaterialInterface* BlankIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UMaterialInterface* CommonIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UMaterialInterface* RareIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UMaterialInterface* EpicIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UMaterialInterface* LegendaryIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UMaterialInterface* BlankRotatedIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UMaterialInterface* CommonRotatedIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UMaterialInterface* RareRotatedIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UMaterialInterface* EpicRotatedIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UMaterialInterface* LegendaryRotatedIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UMaterialInterface* BlankThumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UMaterialInterface* CommonThumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UMaterialInterface* RareThumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UMaterialInterface* EpicThumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UMaterialInterface* LegendaryThumbnail;

	FItemRarityIcon(): BlankIcon(nullptr), CommonIcon(nullptr), RareIcon(nullptr), EpicIcon(nullptr),
	                   LegendaryIcon(nullptr), BlankRotatedIcon(nullptr), CommonRotatedIcon(nullptr),
	                   RareRotatedIcon(nullptr), EpicRotatedIcon(nullptr), LegendaryRotatedIcon(nullptr),
	                   BlankThumbnail(nullptr), CommonThumbnail(nullptr), RareThumbnail(nullptr),
	                   EpicThumbnail(nullptr), LegendaryThumbnail(nullptr)
	{
	}
};


USTRUCT(BlueprintType)
struct FItemRarityImages
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UTexture2D* BlankImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UTexture2D* CommonImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UTexture2D* RareImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UTexture2D* EpicImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UTexture2D* LegendaryImage;

	FItemRarityImages(): BlankImage(nullptr), CommonImage(nullptr), RareImage(nullptr), EpicImage(nullptr),
	                     LegendaryImage(nullptr)
	{
	}
};

USTRUCT(BlueprintType)
struct FItemStatIncrease
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Strength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Armor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Accuracy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CarryWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Humanity;

	FItemStatIncrease(): Health(0), Strength(0), Stamina(0), MoveSpeed(0), Armor(0), Accuracy(0),
	                     CarryWeight(0), Humanity(0)
	{
	}
};

UENUM(BlueprintType)
enum class EPlayerPart : uint8
{
	Basic UMETA(DisplayName = "Basic"),
	Head UMETA(DisplayName = "Head"),
	Body UMETA(DisplayName = "Body"),
	Arm UMETA(DisplayName = "Arm"),
	Leg UMETA(DisplayName = "Leg"),
	Weapon1 UMETA(DisplayName = "Weapon1"),
	Weapon2 UMETA(DisplayName = "Weapon2"),
	Backpack UMETA(DisplayName = "Backpack"),
	ChestRigs UMETA(DisplayName = "ChestRigs")
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Blank UMETA(DisplayName = "Blank"),
	Common UMETA(DisplayName = "Common"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Lengendary")
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FIntPoint Dimensions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Quantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool Stackable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemRarity Rarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemRarityImages RarityImages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemRarityIcon IconStruct;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UMaterialInterface* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UMaterialInterface* RotatedIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UMaterialInterface* EquipedThumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<ACItemBase> ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool Rotated;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FIntPoint StartPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EPlayerPart ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float Weight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	USkeletalMesh* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemStatIncrease StatIncrease;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EWeaponType WeaponType;

	FItemData()
		: ID(0)
		  , Mesh(nullptr)
		  , Dimensions(1, 1)
		  , Quantity(0)
		  , Stackable(false)
		  , Thumbnail(nullptr)
		  , Index(-1)
		  , Rarity(EItemRarity::Common)
		  , RarityImages(FItemRarityImages())
		  , Price(0)
		  , Description(TEXT(""))
		  , Icon(nullptr)
		  , RotatedIcon(nullptr)
		  , ItemClass(nullptr)
		  , Rotated(false)
		  , StartPosition(0, 0)
		  , ItemType(EPlayerPart::Basic)
		  , Weight(1)
		  , SkeletalMesh(nullptr)
		  , StatIncrease(FItemStatIncrease())
		  , ItemName(NAME_None)
	{
	}
};
