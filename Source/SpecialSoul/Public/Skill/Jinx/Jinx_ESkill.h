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
	UJinx_ESkill();
	virtual void UseSkill(ACBasePlayer* Caster) override;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<ARocketLauncherBullet> BulletClass;
	
	UPROPERTY(EditAnywhere, Category = "Skill")
	int32 BulletNum = 5;
	
	UPROPERTY(EditAnywhere, Category = "Skill")
	float FireDelay = 0.05;
	
private:
	FTimerHandle FireTimer;
	
	int32 FiredBulletNum {0};

	float CastingTime {0.5f};
};
