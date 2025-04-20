// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/Projectile.h"
#include "MegaRocketBullet.generated.h"

class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API AMegaRocketBullet : public AProjectile
{
	GENERATED_BODY()


public:
	AMegaRocketBullet();
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
};
