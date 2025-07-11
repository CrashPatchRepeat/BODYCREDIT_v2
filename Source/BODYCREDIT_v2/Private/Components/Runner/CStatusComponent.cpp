#include "Components/Runner/CStatusComponent.h"
#include "Global.h"
#include "Characters/CNox.h"
#include "Characters/Runner/CNox_Runner.h"

UCStatusComponent::UCStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCStatusComponent::TakeDamage(const float Amount)
{
	if (OwnerCharacter && OwnerCharacter->IsA(ACNox_Runner::StaticClass()))
	{
		if (CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter))
		{
			// TODO 플레이어 Enum 추가되면 주석 해제
			// if (CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter)->IsAvoidMode())
			// 	return;
		}
	}

	Health = FMath::Max(0.f, Health - Amount);
	SetHealth(Health);
	if (Health <= 0) Die();
}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UCStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

