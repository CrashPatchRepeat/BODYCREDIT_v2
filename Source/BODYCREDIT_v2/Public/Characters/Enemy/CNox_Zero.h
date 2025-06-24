#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "CNox_Zero.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API ACNox_Zero : public ACNox_EBase
{
	GENERATED_BODY()

public:
#pragma region Skill
	UPROPERTY(EditDefaultsOnly, Category = "Skill CoolDown")
	float MeleeCoolDown = 1.0f;
#pragma endregion
	
private:
};
