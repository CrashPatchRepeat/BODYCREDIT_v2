#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CItemBase.generated.h"

class UCapsuleComponent;
class UCItemObject;
class UCLootInventoryComponent;
class UCItemStrategy;

UCLASS()
class BODYCREDIT_V2_API ACItemBase : public AActor
{
	GENERATED_BODY()

public:
	const FString ContextString{"Item Data"};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Width = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Height = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ItemDataTable;

	UPROPERTY()
	UCItemObject* ItemObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACMainGM* GameMode;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy")
	UCItemStrategy* ItemStrategy;

	UFUNCTION(BlueprintCallable, Category="Item")
	void SetItemStrategy(UCItemStrategy* NewStrategy);

	UFUNCTION(BlueprintCallable, Category="Item")
	void UseItem();

	UCLootInventoryComponent* GetLootInventoryComp() const {return LootInventoryComp;}

protected:
	ACItemBase();
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	UCLootInventoryComponent* LootInventoryComp;
};
