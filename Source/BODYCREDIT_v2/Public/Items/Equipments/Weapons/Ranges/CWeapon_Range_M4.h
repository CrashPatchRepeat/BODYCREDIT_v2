#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/Weapons/Ranges/CWeapon_Range.h"
#include "CWeapon_Range_M4.generated.h"

class USkeletalMeshComponent;
class UStaticMeshComponent;

UCLASS()
class BODYCREDIT_V2_API ACWeapon_Range_M4 : public ACWeapon_Range
{
	GENERATED_BODY()

public:
	ACWeapon_Range_M4();

	void Begin_Equip() override;
	void End_Equip() override;

	void Begin_Aim() override;
	void End_Aim() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* SightMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* ButtMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* ReactoreMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* ForegripMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* MagazineMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName LeftHandSocketName;
};
