#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/Weapons/Ranges/CWeapon_Range.h"
#include "CWeapon_Range_Magnum.generated.h"

UCLASS()
class BODYCREDIT_V2_API ACWeapon_Range_Magnum : public ACWeapon_Range
{
	GENERATED_BODY()
	
public:
	ACWeapon_Range_Magnum();

protected:
	void BeginPlay() override;

public:
	void Begin_Equip() override;

	void Begin_Aim() override;
	void End_Aim() override;
};
