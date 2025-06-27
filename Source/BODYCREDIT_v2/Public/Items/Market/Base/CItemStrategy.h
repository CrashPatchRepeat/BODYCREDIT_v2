#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CItemStrategy.generated.h"

class UCItemObject;
/**
 * 
 */
UCLASS()
class BODYCREDIT_V2_API UCItemStrategy : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void Use(UCItemObject* ItemObject) {};
};
