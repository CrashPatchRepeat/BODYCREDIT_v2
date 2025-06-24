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
	void HealHP(float InHealAmount);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
