#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CNox.generated.h"

UCLASS()
class BODYCREDIT_V2_API ACNox : public ACharacter
{
	GENERATED_BODY()

public:
	ACNox();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
