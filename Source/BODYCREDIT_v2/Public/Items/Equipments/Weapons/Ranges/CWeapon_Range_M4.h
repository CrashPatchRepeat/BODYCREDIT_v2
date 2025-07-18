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

	virtual void BeginPlay() override;

	void Begin_Equip() override;
	void End_Equip() override;

	void Begin_Aim() override;
	void End_Aim() override;

	virtual void OnFiring() override;

	// 탄창 수를 머티리얼에 동기화하는 함수
	virtual void SyncSightAmmoCount() override;

	// 탄창 로드 함수 오버라이드
	virtual void Load_Magazine() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* SightMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* GlassMesh;
	
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

	UMaterialInstanceDynamic* SightMID = nullptr;
};
