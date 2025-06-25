#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAreaTriggerBox.generated.h"

UCLASS()
class BODYCREDIT_V2_API ACAreaTriggerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere, Category="ZoneID")
	FName ZoneID = TEXT("Normal");
	
private:	
	ACAreaTriggerBox();
};
