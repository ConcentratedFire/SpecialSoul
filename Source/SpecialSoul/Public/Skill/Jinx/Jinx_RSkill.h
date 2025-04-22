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
	virtual void UseSkill(ACharacter* Caster) override;

	bool bCasted {false};

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AMegaRocketBullet> BulletClass;

private:
	void StartUseSkill(AJinx* Jinx);
	void EndUseSkill(AJinx* Jinx);
	
	FTimerHandle CastingTimer;
	float CastingTime {0.5f};
};
