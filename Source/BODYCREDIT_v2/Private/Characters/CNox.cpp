#include "Characters/CNox.h"
#include "Global.h"

ACNox::ACNox()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACNox::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACNox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACNox::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACNox::PlayHitStop(float InHitStopTime)
{
	TArray<ACharacter*> characters;
	for (AActor* actor : GetWorld()->GetCurrentLevel()->Actors)
	{
		ACharacter* character = Cast<ACharacter>(actor);

		if (!!character)
		{
			character->CustomTimeDilation = 1e-3f;

			characters.Add(character);
		}
	}

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
		{
			for (ACharacter* character : characters)
				character->CustomTimeDilation = 1;
		});

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, InHitStopTime, false);
}

void ACNox::PlayLaunchCharacter(float InLaunchPower)
{
	FVector start = GetActorLocation();
	FVector target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	FVector direction = target - start;
	direction.Normalize();

	LaunchCharacter(-direction * InLaunchPower, false, false);
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
}
