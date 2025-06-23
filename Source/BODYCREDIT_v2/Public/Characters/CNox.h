#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "CNox.generated.h"

UCLASS()
class BODYCREDIT_V2_API ACNox : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	uint8 GetTeamID() const { return TeamID; }

protected:
	ACNox();
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	uint8 TeamID = 1;	
	
private:
	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }
};
