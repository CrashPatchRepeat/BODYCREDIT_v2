#pragma once

#include "CoreMinimal.h"
#include "Components/CBaseComponent.h"
#include "CNox_FSMComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BODYCREDIT_V2_API UCNox_FSMComp : public UCBaseComponent
{
	GENERATED_BODY()

public:	
	UCNox_FSMComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
