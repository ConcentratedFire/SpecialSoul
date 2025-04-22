#pragma once

#include "CoreMinimal.h"
#include "SkillCooltime.generated.h" 

USTRUCT(BlueprintType)
struct FSkillCooltime
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TotalCooltime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float LeftCooltime;
};
