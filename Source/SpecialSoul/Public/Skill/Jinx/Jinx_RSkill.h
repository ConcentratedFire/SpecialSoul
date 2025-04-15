#pragma once

#include "CoreMinimal.h"
#include "Interface/SkillStrategy.h"
#include "Jinx_RSkill.generated.h"

class AJinx;
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
	
	//UPROPERTY(EditAnywhere, Category = "Skill")
	//int32 BulletNum = 1;
	
	//UPROPERTY(EditAnywhere, Category = "Skill")
	//float FireDelay = 0.1;
	
private:
	void StartUseSkill(AJinx* Jinx);
	void EndUseSkill(AJinx* Jinx);
	
	//FTimerHandle FireTimer;
	//int32 FiredBulletNum {0};

	FTimerHandle CastingTimer;
	float CastingTime {0.5f};
};
