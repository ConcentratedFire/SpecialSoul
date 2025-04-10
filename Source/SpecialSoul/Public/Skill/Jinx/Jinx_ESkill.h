// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SkillStrategy.h"
#include "Jinx_ESkill.generated.h"

class ARocketLauncherBullet;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UJinx_ESkill : public UObject, public ISkillStrategy
{
	GENERATED_BODY()


public:
	virtual void UseSkill(ACBasePlayer* Caster) override;

private:

	float CastingTime {0.5f};

	UPROPERTY(EditAnywhere, Category = "Jinx")
	TSubclassOf<ARocketLauncherBullet> Bullet;
	
};
