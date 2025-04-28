// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/MainBoss/MainBoss.h"
#include "Interface/SkillStrategy.h"
#include "MainBoss_Attack.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UMainBoss_Attack : public UObject, public ISkillStrategy
{
	GENERATED_BODY()

public:
	virtual void UseSkill(ACharacter* Caster) override;
	float AttackDamage {100.f};
};
