#include "Components/Enemy/CNox_EStatusComp.h"

UCNox_EStatusComp::UCNox_EStatusComp()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCNox_EStatusComp::HealHP(float InHealAmount)
{
	SetHealth(FMath::Min(GetMaxHealth(), GetHealth() + InHealAmount));
}

void UCNox_EStatusComp::BeginPlay()
{
	Super::BeginPlay();

}

void UCNox_EStatusComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

