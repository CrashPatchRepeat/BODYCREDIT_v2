#include "Characters/Runner/CNoxAnimInstance_RunnerArms.h"
#include "Global.h"
#include "Characters/Runner/CNox_Runner.h"

void UCNoxAnimInstance_RunnerArms::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	OwnerCharacter = Cast<ACNox_Runner>(OwnerCharacter);
	CheckNull(OwnerCharacter);
	
	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	CheckNull(Weapon);
	
	Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCNoxAnimInstance_RunnerArms::OnWeaponTypeChanged);
}

void UCNoxAnimInstance_RunnerArms::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	CheckNull(OwnerCharacter);
	CheckNull(Weapon);
	
	LeftHandTransform = Weapon->GetLeftHandTransform();
}

void UCNoxAnimInstance_RunnerArms::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}
