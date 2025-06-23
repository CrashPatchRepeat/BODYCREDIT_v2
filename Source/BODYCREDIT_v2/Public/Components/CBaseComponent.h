#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBaseComponent.generated.h"

class ACNox;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BODYCREDIT_V2_API UCBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCBaseComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY()
	ACNox* OwnerCharacter;
	
	virtual void BeginPlay() override;
};
