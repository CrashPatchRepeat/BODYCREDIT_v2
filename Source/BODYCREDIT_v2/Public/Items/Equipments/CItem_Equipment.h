#pragma once

#include "CoreMinimal.h"
#include "Items/CItem.h"
#include "CItem_Equipment.generated.h"

class USkeletalMeshComponent;

UCLASS()
class BODYCREDIT_V2_API ACItem_Equipment : public ACItem
{
	GENERATED_BODY()

public:
	ACItem_Equipment();
	
	virtual void Tick(float DeltaTime) override;

	USkeletalMeshComponent* GetSkeletalMesh() const { return Mesh; }

protected:
	UPROPERTY(visibleanywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* Mesh;
	
	virtual void BeginPlay() override;
};
