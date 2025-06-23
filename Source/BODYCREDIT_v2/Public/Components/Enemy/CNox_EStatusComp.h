#pragma once

#include "CoreMinimal.h"
#include "Components/Runner/CStatusComponent.h"
#include "CNox_EStatusComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BODYCREDIT_V2_API UCNox_EStatusComp : public UCStatusComponent
{
	GENERATED_BODY()

public:	
	UCNox_EStatusComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
