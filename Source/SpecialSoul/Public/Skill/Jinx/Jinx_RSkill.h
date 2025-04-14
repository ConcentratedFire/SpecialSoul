#pragma once

#include "CoreMinimal.h"
#include "Interface/SkillStrategy.h"
#include "Jinx_RSkill.generated.h"

class AMegaRocketBullet;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UJinx_RSkill : public UObject, public ISkillStrategy
{
	GENERATED_BODY()

public:
	UJinx_RSkill();
	virtual void UseSkill(ACBasePlayer* Caster) override;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AMegaRocketBullet> BulletClass;
	
	UPROPERTY(EditAnywhere, Category = "Skill")
	int32 BulletNum = 5;
	
	UPROPERTY(EditAnywhere, Category = "Skill")
	float FireDelay = 0.05;
	
private:
	FTimerHandle FireTimer;
	int32 FiredBulletNum {0};
};
