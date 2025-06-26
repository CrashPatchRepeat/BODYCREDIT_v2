#include "Notifies/CAnimNotify_Reload.h"
#include "Global.h"
#include "Components/Runner/CWeaponComponent.h"

FString UCAnimNotify_Reload::GetNotifyName_Implementation() const
{
	return "Reload";
}

void UCAnimNotify_Reload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	switch (ActionType)
	{
		case EReloadActionType::Eject: weapon->Eject_Magazine(); break;
		case EReloadActionType::Spawn: weapon->Spawn_Magazine(); break;
		case EReloadActionType::Load: weapon->Load_Magazine(); break;
		case EReloadActionType::End: weapon->End_Reload(); break;
	}
}
