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
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	void SetManager(class ACObjectPoolManager* Manager) { ObjectPoolManager = Manager; }

protected:
	virtual void OnDestroy() override;
	
	UPROPERTY()
	TObjectPtr<ACObjectPoolManager> ObjectPoolManager;
};
