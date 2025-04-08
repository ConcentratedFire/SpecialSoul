// Fill out your copyright notice in the Description page of Project Settings.

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
	virtual void UseSkill(AActor* Caster) override;

	UPROPERTY(EditAnywhere, Category = "Jinx")
	TSubclassOf<AMegaRocketBullet> MegaRocketBullet;
	
};
