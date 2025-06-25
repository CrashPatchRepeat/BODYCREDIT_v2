#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "CNox_Medic.generated.h"

class ACElectricGrenade;

/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API ACNox_Medic : public ACNox_EBase
{
	GENERATED_BODY()

public:
#pragma region Grenade
	void HandleElectricGrenade();
	bool IsPlayingGrenade() const;
	void LaunchElectricGrenade();
#pragma endregion

#pragma region Heal
	bool IsLowHealth();
	void HandleHeal(bool bInEquipShield);
	bool IsHealing() const;
#pragma endregion

#pragma region Skill CoolDown
	UPROPERTY(EditDefaultsOnly, Category = "Skill CoolDown")
	float MeleeCoolDown = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Skill CoolDown")
	float HealCoolDown = 20.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Skill CoolDown")
	float GrenadeCoolDown = 10.0f;
#pragma endregion

#pragma region Attack Collision
	virtual void AttackCollision(bool bOn, bool IsRightHand = true) override;
#pragma endregion

private:
#pragma region 생성 및 초기화
	ACNox_Medic();
	virtual void BeginPlay() override;
	virtual void SetPerceptionInfo() override;
#pragma endregion

#pragma region Take Damage
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	                         AController* EventInstigator, AActor* DamageCauser) override;
#pragma endregion

#pragma region Get New Speed
	virtual void GetNewMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed, float& OutNewSpeed,
	                                 float& OutNewAccelSpeed) override;
#pragma endregion

#pragma region 컴포넌트
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* AttackComp_l;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* AttackComp_r;
#pragma endregion

#pragma region Attack Overlap
	UFUNCTION()
	void OnAttackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                   const FHitResult& SweepResult);
#pragma endregion

#pragma region Heal
	UPROPERTY(EditDefaultsOnly, Category=Heal)
	float HealStdValue = 0.4f;
	UPROPERTY(EditDefaultsOnly, Category=Heal)
	class UNiagaraSystem* HealEffectFactory = nullptr;
	UPROPERTY()
	class UNiagaraComponent* HealEffect = nullptr;
#pragma endregion

#pragma region Grenade
	UPROPERTY(VisibleDefaultsOnly)
	TSubclassOf<ACElectricGrenade> ElectricGrenadeCls;
	UPROPERTY(VisibleAnywhere)
	ACElectricGrenade* ElectricGrenade;

	void SuggestProjectileVelocityWithLimit(FVector& OutVelocity,
	                                        const FVector& StartLocation,
	                                        const FVector& TargetLocation,
	                                        float MaxSpeed = 1500.f,
	                                        float GravityZ = -980.f
	);
#pragma endregion
};
