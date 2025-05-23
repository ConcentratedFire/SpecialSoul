// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SkillStrategy.h"
#include "UObject/NoExportTypes.h"
#include "Jinx_Passive.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UJinx_Passive : public UObject, public ISkillStrategy
{
	GENERATED_BODY()

public:
	virtual void UseSkill(ACharacter* Caster) override;
};
