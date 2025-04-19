// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SkillStrategy.h"
#include "MainBoss_DarkinBlade.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UMainBoss_DarkinBlade : public UObject, public ISkillStrategy
{
	GENERATED_BODY()

public:
	virtual void UseSkill(ACharacter* Caster) override;

	int32 ComboCount {0};

	// VFX


	// SFX

	

};
