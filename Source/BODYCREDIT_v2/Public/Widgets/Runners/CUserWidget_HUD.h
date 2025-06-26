#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_HUD.generated.h"

enum class EWeaponType : uint8;

UCLASS()
class BODYCREDIT_V2_API UCUserWidget_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnAutoFire();

	UFUNCTION(BlueprintImplementableEvent)
	void OffAutoFire();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMagazine(uint8 InCurr, uint8 InMax);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWeaponType(EWeaponType InType);
};
