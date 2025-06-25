#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/CItem_Equipment.h"
#include "CEquipment_Weapon.generated.h"

UCLASS()
class BODYCREDIT_V2_API ACEquipment_Weapon : public ACItem_Equipment
{
	GENERATED_BODY()

public:
	ACEquipment_Weapon();

	virtual void Tick(float DeltaTime) override;

protected:	
	virtual void BeginPlay() override;
};
