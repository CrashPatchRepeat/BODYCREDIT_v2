#include "Characters/Enemy/CNox_EBase.h"
#include "Global.h"
#include "Characters/Enemy/CNox_EController.h"
#include "Characters/Enemy/State/CEnemyState.h"
#include "Components/AudioComponent.h"
#include "Components/Enemy/CNox_EStatusComp.h"
#include "Components/Enemy/CNox_FSMComp.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

#pragma region 생성자 및 기본 오버라이드
ACNox_EBase::ACNox_EBase()
{
	TeamID = 2;
}

void ACNox_EBase::BeginPlay()
{
	Super::BeginPlay();
	InitComp();
}

void ACNox_EBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (FSMComp && !bExtractSucceed) FSMComp->UpdateState();
	
	if (bDebug)
	{
		if (FSMComp) // Print Current State
		{
			FString myState = UEnum::GetValueOrBitfieldAsString(FSMComp->GetEnemyState());
			DrawDebugString(GetWorld(), GetActorLocation(), myState, nullptr, FColor::Yellow, 0);
			myState = UEnum::GetValueOrBitfieldAsString(FSMComp->GetCombatState());
			DrawDebugString(
				GetWorld(), FVector(GetActorLocation().X, GetActorLocation().Y,
									GetActorLocation().Z - 50), myState, nullptr, FColor::Yellow, 0);
		}
		if (StatusComp)
		{
			FString myHP = FString::Printf(TEXT("%.2f"), StatusComp->GetHealthPercent());
			DrawDebugString(
				GetWorld(), FVector(GetActorLocation().X, GetActorLocation().Y,
									GetActorLocation().Z - 100), myHP, nullptr, FColor::Red, 0);
		}
	}
}

void ACNox_EBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	EnemyController = Cast<ACNox_EController>(NewController);
}
#pragma endregion

#pragma region Init
void ACNox_EBase::InitComp()
{
	CHelpers::CreateComponent<UAudioComponent>(this, &SoundComponent, "SoundComponent", RootComponent);
	
	GetCapsuleComponent()->SetCollisionProfileName(FName("Enemy"));
	{
		CHelpers::GetClass(&AIControllerClass, TEXT("/Game/Characters/Enemy/AI/BP_NoxController.BP_NoxController_C"));
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}
	{
		// 목표에 거리가 가까워지면서 속도가 줄어드는 현상 방지
		// GetCharacterMovement()->GetNavMovementProperties()->bUseFixedBrakingDistanceForPaths = false;
		GetCharacterMovement()->GetNavMovementProperties()->FixedPathBrakingDistance = 0;
	}
	{
		CHelpers::CreateActorComponent<UCNox_EStatusComp>(this, &StatusComp, "StatusComp");
		CHelpers::CreateActorComponent<UCNox_FSMComp>(this, &FSMComp, "FSMComp");

		// TODO : LootInventoryComp 추가하기		
		// CHelpers::CreateActorComponent<UAC_LootingInventoryComponent>(this, &LootInventoryComp, "LootInventoryComp");
	}
}
#pragma endregion

#pragma region Component
UCNox_EStatusComp* ACNox_EBase::GetStatusComp()
{
	return StatusComp;
}
#pragma endregion

#pragma region Apply Damage
void ACNox_EBase::SetApplyDamage(AActor* DamagedPlayer, const float DamageAmout)
{
	UGameplayStatics::ApplyDamage(DamagedPlayer, DamageAmout, EnemyController, this, UDamageType::StaticClass());
}
#pragma endregion

#pragma region Target
void ACNox_EBase::SetTarget(ACNox* InTarget)
{
	if (StatusComp->GetHealthPercent() <= FLT_MIN) return;
	
	Target = InTarget;
	Target ? FSMComp->SetEnemyState(EEnemyState::Sense) : FSMComp->SetEnemyState(EEnemyState::IDLE);
}
#pragma endregion

#pragma region Set Movement Speed
void ACNox_EBase::SetMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed)
{
	float newSpeed = 0.f, newAccelSpeed = 0.f;
	GetNewMovementSpeed(InMovementSpeed, newSpeed, newAccelSpeed);
	GetCharacterMovement()->MaxWalkSpeed = newSpeed;
	GetCharacterMovement()->MaxAcceleration = newAccelSpeed;
}
#pragma endregion

#pragma region Attacking
void ACNox_EBase::HandleAttack()
{
	// TODO: 공격 처리 구현
	// EnemyAnim->PlayAttackMontage();
}

bool ACNox_EBase::IsAttacking()
{
	// TODO: 공격 중 여부 반환
	// return EnemyAnim->IsAttacking();
	return false;
}
#pragma endregion

#pragma region Hitting
void ACNox_EBase::HandleHit(const int32 sectionIdx)
{
	// TODO: 피격 처리 구현
	// EnemyAnim->PlayHitMontage(sectionIdx);
}

bool ACNox_EBase::IsHitting()
{
	// TODO: 피격 중 여부 반환
	// return EnemyAnim->IsHitting();
	return false;
}

void ACNox_EBase::ResetVal() const
{
	FSMComp->ResetVal(EnemyType);
}
#pragma endregion

#pragma region Die
void ACNox_EBase::HandleDie(const int32 sectionIdx)
{
	SetRagdoll();
	EnemyController->PerceptionDeactive();
}

void ACNox_EBase::SetRagdoll()
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (MeshComp)
	{
		MeshComp->SetCollisionProfileName(TEXT("EnemyDie"));
		MeshComp->SetSimulatePhysics(true);
		MeshComp->WakeAllRigidBodies();
		MeshComp->bBlendPhysics = true;

		// 마지막 피격 위치/본에 임펄스 적용
		if (!LastHitImpulse.IsNearlyZero())
		{
			if (!LastHitBoneName.IsNone())
			{
				MeshComp->AddImpulseAtLocation(LastHitImpulse, LastHitLocation, LastHitBoneName);
			}
			else
			{
				MeshComp->AddImpulse(LastHitImpulse, NAME_None, true);
			}
		}
	}
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (Capsule)
	{
		Capsule->SetCollisionProfileName(FName("EnemyDieCollision"));
	}
}
#pragma endregion

#pragma region Heal (Medic)
void ACNox_EBase::HealHP()
{
	StatusComp->HealHP(HealAmount);
}
#pragma endregion

#pragma region Check Player In Forward Degree
bool ACNox_EBase::IsPlayerInForwardDegree(const float InForwardRange, const float InDegree)
{
	if (!Target) return false;

	const FVector MyLocation = GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();

	// Z축 차이가 100 이상이면 false 리턴
	if (FMath::Abs(MyLocation.Z - TargetLocation.Z) >= 100.f)
	{
		if (bDebug) CLog::Print(TEXT("Z축 차이로 인한 false 반환"));
		return false;
	}

	const float DistanceSquared = FVector::DistSquared(MyLocation, TargetLocation);

	// 거리 제곱으로 비교하여 제곱근 연산 방지
	if (DistanceSquared > FMath::Square(InForwardRange))
	{
		if (bDebug) CLog::Print(FString::Printf(TEXT("DistanceSquared : %f"), DistanceSquared));
		return false;
	}

	const FVector Forward2D = FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0.f).GetSafeNormal();
	TargetLocation.Z = 0.f;
	const FVector ToTarget2D = FVector(TargetLocation - MyLocation).GetSafeNormal2D();

	// 내적 계산 후 각도 변환
	const float Dot = FMath::Clamp(FVector::DotProduct(Forward2D, ToTarget2D), -1.f, 1.f);
	return FMath::RadiansToDegrees(FMath::Acos(Dot)) <= InDegree;
}
#pragma endregion

#pragma region FSM Set State
void ACNox_EBase::SetEnemyState(EEnemyState NewState)
{
	FSMComp->SetEnemyState(NewState);
}

void ACNox_EBase::SetCombatState(ECombatState NewCombatState)
{
	FSMComp->SetCombatState(NewCombatState);
}
#pragma endregion

#pragma region FSM Skill Cool Downs
void ACNox_EBase::UpdateSkillCoolDowns(ESkillCoolDown Skill, float DeltaTime)
{
	FSMComp->UpdateSkillCoolDowns(Skill, DeltaTime);
}

bool ACNox_EBase::IsSkillReady(ESkillCoolDown Skill) const
{
	return FSMComp->IsSkillReady(Skill);
}

void ACNox_EBase::UsingSkill(ESkillCoolDown Skill)
{
	FSMComp->UsingSkill(Skill);
}
#pragma endregion

#pragma region Rotate To Target
bool ACNox_EBase::RotateToTarget(const float DeltaTime, const FTransform& CurTrans, const FVector& TargetLoc, float InteropSpeed)
{
	// 1. 현재 위치와 목표 위치를 2D로 변환
	const FVector CurrentLocation = CurTrans.GetLocation();
	const FVector DirectionToTarget = (TargetLoc - CurrentLocation).GetSafeNormal2D();

	// 2. 목표 회전값 계산
	const FRotator TargetRotation = DirectionToTarget.Rotation();

	// 3. 현재 회전값 가져오기
	const FRotator CurrentRotation = CurTrans.GetRotation().Rotator();

	// 4. Yaw만 보간
	const float NewYaw = FMath::FInterpTo(
		CurrentRotation.Yaw,
		TargetRotation.Yaw,
		DeltaTime,
		InteropSpeed
	);

	// 5. 새로운 회전값 설정 (Pitch와 Roll은 유지)
	const FRotator NewRotation(CurrentRotation.Pitch, NewYaw, CurrentRotation.Roll);
	SetActorRotation(NewRotation);

	return true;
}

void ACNox_EBase::SetRotateToTarget()
{
	// 1. 현재 회전값 가져오기
	const FRotator CurrentRotation = GetActorRotation();

	// 2. 목표 방향 계산
	const FVector DirectionToTarget = (GetTarget()->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
	const float TargetYaw = DirectionToTarget.Rotation().Yaw;

	// 3. 새로운 회전값 설정 (Pitch와 Roll은 유지)
	const FRotator NewRotation(CurrentRotation.Pitch, TargetYaw, CurrentRotation.Roll);
	SetActorRotation(NewRotation);
}
#pragma endregion

#pragma region Extract Call Function
void ACNox_EBase::ExtractCallFunction(ACNox* InTarget)
{
	RetentionTime = 0.f; // 타겟 잃어버림 방지용, 컨트롤러에서 RetentionTime으로 잊게 해놈
	SetTarget(InTarget);
}
#pragma endregion

#pragma region Sound
void ACNox_EBase::PlayIdleSound()
{
	if (!IdleSoundCue)
		return;

	if (SoundComponent->Sound != IdleSoundCue)
	{
		SoundComponent->SetSound(IdleSoundCue);
		SoundComponent->Play();
	}
	else if (!SoundComponent->IsPlaying())
	{
		SoundComponent->Play();
	}
}
void ACNox_EBase::PlaySenseSound()
{
	if (!SenseSoundCue)
		return;

	if (SoundComponent->Sound != SenseSoundCue)
	{
		SoundComponent->SetSound(SenseSoundCue);
		SoundComponent->Play();
	}
	else if (!SoundComponent->IsPlaying())
	{
		SoundComponent->Play();
	}
}
void ACNox_EBase::PlayAttackSound()
{
	if (!AttackSoundCue)
		return;

	if (SoundComponent->Sound != AttackSoundCue)
	{
		SoundComponent->SetSound(AttackSoundCue);
		SoundComponent->Play();
	}
	else if (!SoundComponent->IsPlaying())
	{
		SoundComponent->Play();
	}
}
void ACNox_EBase::PlayGrenadeSound()
{
	if (!GrenadeSoundCue)
		return;

	if (SoundComponent->Sound != GrenadeSoundCue)
	{
		SoundComponent->SetSound(GrenadeSoundCue);
		SoundComponent->Play();
	}
	else if (!SoundComponent->IsPlaying())
	{
		SoundComponent->Play();
	}
}
void ACNox_EBase::PlayHealSound()
{
	if (!HealSoundCue)
		return;

	if (SoundComponent->Sound != HealSoundCue)
	{
		SoundComponent->SetSound(HealSoundCue);
		SoundComponent->Play();
	}
	else if (!SoundComponent->IsPlaying())
	{
		SoundComponent->Play();
	}
}
void ACNox_EBase::PlayBeamSound()
{
	if (!BeamSoundCue)
		return;

	if (SoundComponent->Sound != BeamSoundCue)
	{
		SoundComponent->SetSound(BeamSoundCue);
		SoundComponent->Play();
	}
	else if (!SoundComponent->IsPlaying())
	{
		SoundComponent->Play();
	}
}
void ACNox_EBase::PlayWavePulseSound()
{
	if (!WavePulseSoundCue)
		return;

	if (SoundComponent->Sound != WavePulseSoundCue)
	{
		SoundComponent->SetSound(WavePulseSoundCue);
		SoundComponent->Play();
	}
	else if (!SoundComponent->IsPlaying())
	{
		SoundComponent->Play();
	}
}
void ACNox_EBase::PlayHitSound()
{
	if (!HitSoundCue)
		return;

	if (SoundComponent->Sound != HitSoundCue)
	{
		SoundComponent->SetSound(HitSoundCue);
		SoundComponent->Play();
	}
	else if (!SoundComponent->IsPlaying())
	{
		SoundComponent->Play();
	}
}
void ACNox_EBase::PlayDieSound()
{
	if (!DieSoundCue)
		return;

	if (SoundComponent->Sound != DieSoundCue)
	{
		SoundComponent->SetSound(DieSoundCue);
		SoundComponent->Play();
	}
	else if (!SoundComponent->IsPlaying())
	{
		SoundComponent->Play();
	}
}
#pragma endregion
