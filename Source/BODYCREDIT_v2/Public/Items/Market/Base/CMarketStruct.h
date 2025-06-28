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
	TObjectPtr<UMaterialInterface> BlankIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UMaterialInterface> CommonIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UMaterialInterface> RareIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UMaterialInterface> EpicIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UMaterialInterface> LegendaryIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UMaterialInterface> BlankRotatedIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UMaterialInterface> CommonRotatedIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UMaterialInterface> RareRotatedIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UMaterialInterface> EpicRotatedIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UMaterialInterface> LegendaryRotatedIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UMaterialInterface> BlankThumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UMaterialInterface> CommonThumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UMaterialInterface> RareThumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UMaterialInterface> EpicThumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UMaterialInterface> LegendaryThumbnail;

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
	TObjectPtr<UTexture2D> BlankImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UTexture2D> CommonImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UTexture2D> RareImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UTexture2D> EpicImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	TObjectPtr<UTexture2D> LegendaryImage;

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
	
	// 포인터 타입들 (8바이트 정렬)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "2"))
	TObjectPtr<UStaticMesh> Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "4"))
	TObjectPtr<UTexture2D> Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "10"))
	TObjectPtr<UMaterialInterface> Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "11"))
	TObjectPtr<UMaterialInterface> RotatedIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "12"))
	TObjectPtr<UMaterialInterface> EquipedThumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "18"))
	TObjectPtr<USkeletalMesh> SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "13"))
	TSubclassOf<ACItemBase> ItemClass;
	
	// 구조체들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "6"))
	FItemRarityImages RarityImages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "9"))
	FItemRarityIcon IconStruct;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "19"))
	FItemStatIncrease StatIncrease;
	
	// 정수 타입들 (4바이트 정렬)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "1"))
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "7"))
	int32 Price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "3"))
	FIntPoint Dimensions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "15"))
	FIntPoint StartPosition;
	
	// 열거형들 (1바이트)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "5"))
	EItemRarity Rarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "16"))
	EPlayerPart ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "21"))
	EWeaponType WeaponType;
	
	// 부울 타입 (1바이트)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "14"))
	bool Rotated;
	
	// 문자열과 이름 (마지막에 배치)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "8"))
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "20"))
	FName ItemName;
	
	// 실수 타입 (4바이트 정렬)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayPriority = "17"))
	float Weight;

	FItemData()
		: Mesh(nullptr)
		  , Thumbnail(nullptr)
		  , Icon(nullptr)
		  , RotatedIcon(nullptr)
		  , EquipedThumbnail(nullptr)
		  , SkeletalMesh(nullptr)
		  , ItemClass(nullptr)
		  , RarityImages(FItemRarityImages())
		  , IconStruct(FItemRarityIcon())
		  , StatIncrease(FItemStatIncrease())
		  , ID(0)
		  , Price(0)
		  , Dimensions(1, 1)
		  , StartPosition(0, 0)
		  , Rarity(EItemRarity::Common)
		  , ItemType(EPlayerPart::Basic)
		  , WeaponType(EWeaponType::Max)
		  , Rotated(false)
		  , Description(TEXT(""))
		  , ItemName(NAME_None)
		  , Weight(1)
	{
	}
};
