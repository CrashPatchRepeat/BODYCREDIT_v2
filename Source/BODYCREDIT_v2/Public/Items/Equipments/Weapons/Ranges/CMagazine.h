#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMagazine.generated.h"

class USceneComponent;
class USkeletalMeshComponent;

UCLASS()
class BODYCREDIT_V2_API ACMagazine : public AActor
{
	GENERATED_BODY()
	
public:	
	ACMagazine();

	virtual void Tick(float DeltaTime) override;
	
	void SetEject();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh_Full;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh_Empty;
	
	bool bEject;
};
