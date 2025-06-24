#include "Characters/CNoxAnimInstance.h"
#include "Global.h"
#include "Characters/CNox.h"

void UCNoxAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACNox>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);
}

void UCNoxAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CheckNull(OwnerCharacter);
}
