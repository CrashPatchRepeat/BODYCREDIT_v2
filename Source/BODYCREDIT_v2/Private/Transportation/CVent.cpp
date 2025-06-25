#include "Transportation/CVent.h"
#include "Global.h"

ACVent::ACVent()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root", RootComponent);
}
