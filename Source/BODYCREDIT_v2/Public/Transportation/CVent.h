#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CVent.generated.h"

UCLASS()
class BODYCREDIT_V2_API ACVent : public AActor
{
	GENERATED_BODY()

public:
	ACVent();
	FVector GetLocation() const {return GetActorLocation();}

private:
	UPROPERTY(VisibleAnywhere, Category = "Vent")
	USceneComponent* Root;
};
