// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SkillStrategy.h"
#include "CYasuo_RSkill.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCYasuo_RSkill : public UObject, public ISkillStrategy
{
	GENERATED_BODY()
	
public:
	UCYasuo_RSkill();
	virtual void UseSkill(ACharacter* Caster) override;

private:
	UPROPERTY()
	class ACYasuo* Yasuo;
	
	void StartUseSkill();

	FTimerHandle FireTimer;
	UPROPERTY(EditAnywhere, Category = "Skill")
	float FireDelay = 0.1f;

	bool bCanSkillActive = true;
	float ChargeCooldown = 20.f;
	FTimerHandle ChargeTimerHandle; // 충전 타이머

	UFUNCTION()
	void OnChargeCompleted();
};
