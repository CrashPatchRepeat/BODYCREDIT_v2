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

	virtual void AttackCollision(bool bOn, bool IsRightHand = true) override;
	
private:
	ACNox_Zero();
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetNewMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed, float& OutNewSpeed,
									 float& OutNewAccelSpeed) override;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* AttackComp_l;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* AttackComp_r;
	UFUNCTION()
	void OnAttackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
									   const FHitResult& SweepResult);
};
