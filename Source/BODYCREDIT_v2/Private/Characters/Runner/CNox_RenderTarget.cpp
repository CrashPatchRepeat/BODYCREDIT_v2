#include "Characters/Runner/CNox_RenderTarget.h"
#include "Global.h"

ACNox_RenderTarget::ACNox_RenderTarget()
{
	InitCharacterMeshes();
}

void ACNox_RenderTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACNox_RenderTarget::BeginPlay()
{
	Super::BeginPlay();
}

void ACNox_RenderTarget::InitCharacterMeshes()
{
	USkeletalMesh* mesh;

	// Head
	CHelpers::GetAsset(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Runner/Meshes/Scifi_Character05/Scifi_Character05_Head.Scifi_Character05_Head'"));
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	// Hair
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Hair, TEXT("Hair"), GetMesh());
	CHelpers::GetAsset(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Runner/Meshes/Scifi_Character03/Scifi_Character03_Hair.Scifi_Character03_Hair'"));
	Hair->SetSkeletalMesh(mesh);
	
	// UpperBody
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &UpperBody, TEXT("UpperBody"), GetMesh());
	CHelpers::GetAsset(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Runner/Meshes/Scifi_Character03/Scifi_Character03_TorsoA.Scifi_Character03_TorsoA'"));
	UpperBody->SetSkeletalMesh(mesh);
	
	// Outer
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Outer, TEXT("Outer"), GetMesh());
	CHelpers::GetAsset(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Runner/Meshes/Scifi_Character03/Scifi_Character03_TorsoB.Scifi_Character03_TorsoB'"));
	Outer->SetSkeletalMesh(mesh);
	
	// Arms
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Arms, TEXT("Arms"), GetMesh());
	CHelpers::GetAsset(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Runner/Meshes/Scifi_Character02/Scifi_Character02_Arms.Scifi_Character02_Arms'"));
	Arms->SetSkeletalMesh(mesh);
	
	// LowerBody
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &LowerBody, TEXT("LowerBody"), GetMesh());
	CHelpers::GetAsset(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Runner/Meshes/Scifi_Character04/Scifi_Character04_Legs.Scifi_Character04_Legs'"));
	LowerBody->SetSkeletalMesh(mesh);
	
	// Foot
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Foot, TEXT("Foot"), GetMesh());
	CHelpers::GetAsset(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Runner/Meshes/Scifi_Character03/Scifi_Character03_Foot.Scifi_Character03_Foot'"));
	Foot->SetSkeletalMesh(mesh);
	
	// ChestRig
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &ChestRig, TEXT("ChestRig"), GetMesh());
	
	// Backpack
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Backpack, TEXT("Backpack"), GetMesh());
	CHelpers::GetAsset(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Runner/Meshes/Scifi_Character01/Scifi_Character01_Bag.Scifi_Character01_Bag'"));
	Backpack->SetSkeletalMesh(mesh);

	//WeaponSlot1
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &WeaponSlot1, TEXT("WeaponSlot1"), GetMesh());
	WeaponSlot1->SetHiddenInGame(true);

	//WeaponSlot2
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &WeaponSlot2, TEXT("WeaponSlot2"), GetMesh());
	WeaponSlot2->SetHiddenInGame(true);

	// LeaderPoseComponent 설정 및 AnimSequence 할당
	UAnimSequence* renderTargetAnimSequence = nullptr;
	CHelpers::GetAsset(&renderTargetAnimSequence, TEXT("/Script/Engine.AnimSequence'/Game/Characters/Runner/Animations/RenderTarget/RenderTarget.RenderTarget'"));

	USkeletalMeshComponent* leader = GetMesh();
	if (renderTargetAnimSequence)
	{
		leader->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		leader->SetAnimation(renderTargetAnimSequence);
	}

	TArray<USkeletalMeshComponent*> parts = { Hair, UpperBody, Outer, Arms, LowerBody, Foot, ChestRig, Backpack, WeaponSlot1, WeaponSlot2 };
	for (USkeletalMeshComponent* part : parts)
	{
		if (part)
		{
			part->SetLeaderPoseComponent(leader);
			if (renderTargetAnimSequence)
			{
				part->SetAnimationMode(EAnimationMode::AnimationSingleNode);
				part->SetAnimation(renderTargetAnimSequence);
			}
		}
	}
}
