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
	ACYasuo();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Attack() override;

private:
	int32 MP = 0;
	bool bUseAOE = false;
	float AOERange = 0.f;
	float AOEDamage = 0.f;
	float AOECoolTime = 0.f;

private: // Attack
	UPROPERTY(EditDefaultsOnly, category=Attack)
	TSubclassOf<class ACTornado> TornadoFactory;
	UPROPERTY(VisibleAnywhere, Category=Attack)
	int32 AttackCnt = 3;

	TArray<FVector> GetAttackVector();

private: // Anim
	UPROPERTY()
	class UCYasuoAnim* Anim;
};
