// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "CMeleeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ACMeleeEnemy : public ABaseEnemy
{
	GENERATED_BODY()

private:
	ACMeleeEnemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void DieEndAction() override;
};
