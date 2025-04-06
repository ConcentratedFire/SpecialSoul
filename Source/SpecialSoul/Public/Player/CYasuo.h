// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/CBasePlayer.h"
#include "CYasuo.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ACYasuo : public ACBasePlayer
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Attack() override;

private:
	bool bUseAOE = false;
	float AOERange = 0.f;
	float AOEDamage = 0.f;
	float AOECoolTime = 0.f;
};
