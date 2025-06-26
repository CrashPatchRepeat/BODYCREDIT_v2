#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/Weapons/Ranges/CWeapon_Range.h"
#include "CWeapon_Range_AR.generated.h"

class USkeletalMeshComponent;

UCLASS()
class BODYCREDIT_V2_API ACWeapon_Range_AR : public ACWeapon_Range
{
	GENERATED_BODY()

public:
	ACWeapon_Range_AR();

	void Begin_Equip() override;
	void End_Equip() override;

	void Begin_Aim() override;
	void End_Aim() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* SightMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* ButtMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* MagazineMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName LeftHandSocketName;
};
