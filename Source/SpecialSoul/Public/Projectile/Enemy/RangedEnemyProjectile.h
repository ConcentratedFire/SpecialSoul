// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/Projectile.h"
#include "RangedEnemyProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ARangedEnemyProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	ARangedEnemyProjectile();
	virtual void Tick(float DeltaSeconds) override;


protected:
	virtual void BeginPlay() override;
	virtual void ApplyLifeTime() override;
	virtual void InitMoveComp() override;
};
