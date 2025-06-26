#include "Characters/Enemy/AttackActor/CBeam.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "Characters/Enemy/CNox_Memory.h"
#include "Characters/Runner/CNox_Runner.h"

ACBeam::ACBeam()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<USceneComponent>(this, &rootScene, "rootScene");
	CHelpers::CreateComponent<UNiagaraComponent>(this, &LaserBeamVFX, "LaserBeamVFX", RootComponent);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &FireBallVFX, "FireBallVFX", RootComponent);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &HitVFX, "HitVFX", RootComponent);
}

void ACBeam::BeginPlay()
{
	Super::BeginPlay();
	// VFX 초기화
	DeactivateAllVFX();
	// AI 컨트롤러 설정
	OwnerAI = Cast<ACNox_Memory>(GetOwner());
}

void ACBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateDamageTimer(DeltaTime);
	
	if (TargetActor && AttackStart)
	{
		UpdateBeamPosition(DeltaTime);
		UpdateAttackDelay(DeltaTime);
	}
}

void ACBeam::SetBeamActive(bool bInActive, AActor* InTarget)
{
	SetActorHiddenInGame(!bInActive);
	TargetActor = InTarget;
	
	if (bInActive)
	{
		InitializeBeam(InTarget);
	}
	else
	{
		DeactivateBeam();
	}
}

#pragma region 컴포넌트 생성 및 초기화
void ACBeam::CreateComponents()
{
	CHelpers::CreateComponent<USceneComponent>(this, &rootScene, "rootScene");
	CHelpers::CreateComponent<UNiagaraComponent>(this, &LaserBeamVFX, "LaserBeamVFX", RootComponent);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &FireBallVFX, "FireBallVFX", RootComponent);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &HitVFX, "HitVFX", RootComponent);
}

void ACBeam::SetupVFXAssets()
{
	// 레이저 빔 VFX 설정
	UNiagaraSystem* NiagaraSys = nullptr;
	CHelpers::GetAsset<UNiagaraSystem>(&NiagaraSys, TEXT("/Game/Assets/3D_Lasers/Effects/NS_Beam_3.NS_Beam_3"));
	if (NiagaraSys)
	{
		LaserBeamVFX->SetAsset(NiagaraSys);
		LaserBeamVFX->SetAutoActivate(false);
	}

	// 파이어볼 VFX 설정
	CHelpers::GetAsset<UNiagaraSystem>(&NiagaraSys, TEXT("/Game/Assets/LaserBeam/Niagara/FireBall/NS_FireBall5.NS_FireBall5"));
	if (NiagaraSys)
	{
		FireBallVFX->SetAsset(NiagaraSys);
		FireBallVFX->SetAutoActivate(false);
	}

	// 히트 VFX 설정
	CHelpers::GetAsset<UNiagaraSystem>(&NiagaraSys, TEXT("/Game/Assets/3D_Lasers/Effects/NS_LaserHit_3.NS_LaserHit_3"));
	if (NiagaraSys)
	{
		HitVFX->SetAsset(NiagaraSys);
		HitVFX->SetAutoActivate(false);
	}
}

void ACBeam::DeactivateAllVFX()
{
	LaserBeamVFX->DeactivateImmediate();
	FireBallVFX->Deactivate();
	HitVFX->DeactivateImmediate();
}
#pragma endregion

#pragma region 빔 활성화/비활성화
void ACBeam::InitializeBeam(AActor* InTarget)
{
	DamageTimer = 0.0f;
	bApplyDamage = false;
	
	CurPitch = (InTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation().Pitch;
	FireBallVFX->Activate(true);

	GetWorldTimerManager().SetTimer(ActiveTimer, [this]()
	{
		AttackStart = true;
		LaserBeamVFX->Activate(true);
		HitVFX->Activate(true);
	}, ActiveDelay, false);
}

void ACBeam::DeactivateBeam()
{
	AttackStart = false;
	DeactivateAllVFX();
}
#pragma endregion

#pragma region 빔 업데이트
void ACBeam::UpdateDamageTimer(float DeltaTime)
{
	if (bApplyDamage)
	{
		if (DamageTimer <= DamageInterval)
		{
			DamageTimer += DeltaTime;
		}
		else
		{
			bApplyDamage = false;
			DamageTimer = 0.f;
		}
	}
}

void ACBeam::UpdateBeamPosition(float DeltaTime)
{
	FVector start = rootScene->GetComponentLocation();
	FVector end = CalculateBeamEnd(DeltaTime);
	
	// 레이캐스트 수행
	FHitResult HitResult;
	if (PerformLineTrace(start, end, HitResult))
	{
		HandleBeamHit(HitResult);
	}
	else
	{
		HandleBeamMiss(end);
	}
}

void ACBeam::UpdateAttackDelay(float DeltaTime)
{
	CurAttackDelay += DeltaTime;
	if (CurAttackDelay >= AttackDelay)
	{
		CurAttackDelay = 0.f;
	}
}
#pragma endregion

#pragma region 빔 계산
FVector ACBeam::CalculateBeamEnd(float DeltaTime)
{
	// 피치 각도 계산
	float newPitch = FMath::Lerp(
		CurPitch, 
		(TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation().Pitch,
		DeltaTime * 3
	);
	CurPitch = newPitch;

	// 회전값 계산
	FRotator NewRotation = FRotator(newPitch, 0, 0);
	if (!OwnerAI) return FVector::ZeroVector;
	FRotator aiRot = OwnerAI->GetActorRotation();
	if (((180 > aiRot.Yaw && aiRot.Yaw > 90) || (-90 > aiRot.Yaw && aiRot.Yaw > -180))) 
		NewRotation.Pitch *= -1;

	// 빔 방향 계산
	FVector Direction = NewRotation.RotateVector(OwnerAI->GetActorForwardVector() * AttackRange);
	return rootScene->GetComponentLocation() + Direction;
}

bool ACBeam::PerformLineTrace(const FVector& Start, const FVector& End, FHitResult& OutHitResult)
{
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(OwnerAI);
	
	return GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECC_Visibility, params);
}
#pragma endregion

#pragma region 빔 충돌
void ACBeam::HandleBeamHit(const FHitResult& HitResult)
{
	LaserBeamVFX->SetVariableVec3(FName("User.LaserEnd"), HitResult.Location);
	
	if (!HitVFX->IsActive()) 
		HitVFX->Activate(true);
	HitVFX->SetWorldLocation(HitResult.Location + 10);

	if (!bApplyDamage && HitResult.GetActor()->IsA(ACNox_Runner::StaticClass()))
	{
		OwnerAI->SetApplyDamage(HitResult.GetActor(), 10.f);
		bApplyDamage = true;
	}
}

void ACBeam::HandleBeamMiss(const FVector& EndLocation)
{
	LaserBeamVFX->SetVariableVec3(FName("User.LaserEnd"), EndLocation);
	HitVFX->DeactivateImmediate();
}
#pragma endregion

