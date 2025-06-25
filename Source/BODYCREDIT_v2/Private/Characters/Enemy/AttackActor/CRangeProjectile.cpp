#include "Characters/Enemy/AttackActor/CRangeProjectile.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "Characters/Enemy/CNox_Memory.h"
#include "Components/BoxComponent.h"

ACRangeProjectile::ACRangeProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<UBoxComponent>(this, &BoxComp, "BoxComp");
	BoxComp->SetCollisionProfileName(FName("EnemyWeapon"));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACRangeProjectile::OnOverlap);

	CHelpers::CreateComponent<UNiagaraComponent>(this, &ProjectileFxComp, "ProjectileFxComp", RootComponent);
}

void ACRangeProjectile::BeginPlay()
{
	Super::BeginPlay();
	OwnerAI = Cast<ACNox_Memory>(GetOwner());
}

void ACRangeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Target)
	{
		FVector ToTarget = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FVector CurrentDir = CurrentVelocity.GetSafeNormal();

		// 회전 제한 걸기
		FVector NewDir = FMath::VInterpConstantTo(CurrentDir, ToTarget, DeltaTime, MaxTurnRateDegPerSec);
		NewDir = NewDir.GetSafeNormal();

		CurrentVelocity = NewDir * Speed;
		SetActorLocationAndRotation(GetActorLocation() + CurrentVelocity * DeltaTime, NewDir.Rotation());
	}
}

void ACRangeProjectile::InitializeProjectile(FVector InStartLocation, class ACNox* InTargetActor)
{
	Target = InTargetActor;
	StartLocation = InStartLocation;
	SetActorLocation(StartLocation);

	// 처음에는 정면으로 쏨
	FVector Direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	CurrentVelocity = Direction * Speed;

	ProjectileFxComp->ReinitializeSystem(); // 모든 파티클 제거 + 재시작
}

void ACRangeProjectile::SetCollisionEnabled(bool bEnabled)
{
	BoxComp->SetCollisionEnabled(bEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}

void ACRangeProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACNox::StaticClass()))
		OwnerAI->SetApplyDamage(OtherActor, 10.f);
	OwnerAI->ReturnToPool(this);
}

