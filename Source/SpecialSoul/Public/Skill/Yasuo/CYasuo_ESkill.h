// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SkillStrategy.h"
#include "Player/CYasuo.h"
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
	UPROPERTY()
	class ACYasuo* Yasuo;
	
	void StartUseSkill();
	void Dash(FVector EndPos);
	void EndUseSkill();

	UPROPERTY(EditAnywhere, Category = "Skill")
	float MoveDist = 300.f;
	UPROPERTY(EditAnywhere, Category = "Skill")
	float MoveThreshold = 10.f;

	FTimerHandle FireTimer;
	UPROPERTY(EditAnywhere, Category = "Skill")
	float FireDelay = 0.1f;

	float DashTime = 0.f;

	UPROPERTY(EditAnywhere, Category = "Skill")
	int32 MaxChargeCount{3};
	int32 SkillChargeCount{MaxChargeCount};
	bool bIsESkillActive = false;
	float ChargeCooldown = 20.f;
	FTimerHandle ChargeTimerHandle; // 충전 타이머

	void StartChargeTimer();
	UFUNCTION()
	void OnChargeCompleted();
};