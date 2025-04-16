// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SkillStrategy.h"
#include "Player/CYasuo.h"
#include "UObject/NoExportTypes.h"
#include "CYasuo_ESkill.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCYasuo_ESkill : public UObject, public ISkillStrategy
{
	GENERATED_BODY()

public:
	UCYasuo_ESkill();
	virtual void UseSkill(ACBasePlayer* Caster) override;

private:
	void StartUseSkill(ACYasuo* Yasuo);
	void EndUseSkill(ACYasuo* Yasuo);

	UPROPERTY(EditAnywhere, Category = "Skill")
	float MoveDist = 300.f;
	UPROPERTY(EditAnywhere, Category = "Skill")
	float MoveThreshold = 10.f;
	
	FTimerHandle CastingTimer;
	float CastingTime = 0.1f;

	FTimerHandle FireTimer;
	UPROPERTY(EditAnywhere, Category = "Skill")
	float FireDelay = 0.1f;
};
