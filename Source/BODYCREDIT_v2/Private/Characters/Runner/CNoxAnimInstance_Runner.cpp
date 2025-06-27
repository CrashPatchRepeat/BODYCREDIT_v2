#include "Characters/Runner/CNoxAnimInstance_Runner.h"
#include "Global.h"
#include "Characters/Runner/CNox_Runner.h"
#include "Components/Runner/CWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCNoxAnimInstance_Runner::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACNox_Runner>(OwnerCharacter);
	CheckNull(OwnerCharacter);
	
	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	CheckNull(Weapon);

	Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCNoxAnimInstance_Runner::OnWeaponTypeChanged);
}

void UCNoxAnimInstance_Runner::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CheckNull(OwnerCharacter);

	// 캐릭터 이동 속도 ( 0 ~ )
	Speed = OwnerCharacter->GetVelocity().Size2D();

	// 방향 보간 ( -180 ~ +180 )
	FRotator rot1 = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rot2 = OwnerCharacter->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rot1, rot2);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);
	Direction = PrevRotation.Yaw;

	bIsCrouching = OwnerCharacter->bIsCrouched;

	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();
	
	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);

	CheckNull(Weapon);
	
	bInAim = Weapon->IsInAim();
	bUseIK = (Weapon->IsUnarmedMode() == false);
	LeftHandLocation = Weapon->GetLeftHandLocation();
}

bool UCNoxAnimInstance_Runner::IsRunning()
{
	CheckTrueResult(FMath::IsNearlyZero(Speed, 0.01f), false);
	CheckTrueResult(Speed > 500.0f, false);

	return true;
}

bool UCNoxAnimInstance_Runner::IsSprinting()
{
	CheckTrueResult(FMath::IsNearlyZero(Speed, 0.01f), false);
	CheckTrueResult(Speed <= 500.0f, false);

	return true;
}

void UCNoxAnimInstance_Runner::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}
