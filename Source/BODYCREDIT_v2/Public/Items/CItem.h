#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CItem.generated.h"

class USceneComponent;
class ACNox;

UCLASS()
class BODYCREDIT_V2_API ACItem : public AActor
{
	GENERATED_BODY()

public:
	ACItem();

	virtual void Tick(float DeltaTime) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USceneComponent* Root;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "OwnerCharacter")
	ACNox_Runner* OwnerCharacter;
	
	virtual void BeginPlay() override;
};
