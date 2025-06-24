#include "Characters/Runner/CNoxAnimInstance_Runner.h"
#include "Global.h"
#include "Characters/Runner/CNox_Runner.h"

void UCNoxAnimInstance_Runner::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACNox_Runner>(OwnerCharacter);
	CheckNull(OwnerCharacter);
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
