// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillStrategy.generated.h"

class ACBasePlayer;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USkillStrategy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPECIALSOUL_API ISkillStrategy
{
	GENERATED_BODY()

public:
	virtual void UseSkill(ACBasePlayer* Caster) = 0;
};
