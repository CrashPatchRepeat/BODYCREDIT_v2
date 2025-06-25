#include "Trigger/CAreaTriggerBox.h"
#include "Global.h"
#include "Components/BoxComponent.h"

ACAreaTriggerBox::ACAreaTriggerBox()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<UBoxComponent>(this, &BoxComp, "BoxComp");
	BoxComp->SetCollisionProfileName(FName("Zone"));
}
