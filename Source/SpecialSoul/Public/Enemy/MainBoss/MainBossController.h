// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/AI/CEnemyController.h"
#include "MainBossController.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API AMainBossController : public ACEnemyController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetActorTickEnabled(bool bEnabled) override;
};
