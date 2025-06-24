#pragma once

#include "CoreMinimal.h"
#include "Components/CBaseComponent.h"
#include "CStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BODYCREDIT_V2_API UCStatusComponent : public UCBaseComponent
{
	GENERATED_BODY()

public:	
	UCStatusComponent();

#pragma region Health
	void SetHealth(const float InHealth) {Health = InHealth;}
	float GetHealth() const { return Health; }
	float GetMaxHealth() const { return MaxHealth; }
	float GetHealthPercent() const { return Health / MaxHealth; }
#pragma endregion

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
#pragma region Health
	UPROPERTY(EditDefaultsOnly, Category="Status")
	float MaxHealth{500};
	UPROPERTY(VisibleAnywhere, Category="Status")
	float Health;
#pragma endregion
};
