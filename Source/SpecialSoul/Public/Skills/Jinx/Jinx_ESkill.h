// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/SkillStrategy.h"
#include "UObject/NoExportTypes.h"
#include "Jinx_ESkill.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UJinx_ESkill : public UObject, public ISkillStrategy
{
	GENERATED_BODY()


public:
	virtual void UseSkill(AActor* Caster) override;
};
