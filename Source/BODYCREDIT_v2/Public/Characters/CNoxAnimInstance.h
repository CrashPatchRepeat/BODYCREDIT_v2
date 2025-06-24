#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CNoxAnimInstance.generated.h"

class ACNox;

UCLASS()
class BODYCREDIT_V2_API UCNoxAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay();

	virtual void NativeUpdateAnimation(float DeltaSeconds);

protected:
	UPROPERTY(blueprintreadonly, Category = "Owner")
	ACNox* OwnerCharacter;
};
