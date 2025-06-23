#include "Components/Enemy/CNox_FSMComp.h"

UCNox_FSMComp::UCNox_FSMComp()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCNox_FSMComp::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCNox_FSMComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

