// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/Projectile.h"
#include "MinigunBullet.generated.h"

class UProjectileMovementComponent;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API AMinigunBullet : public AProjectile
{
	GENERATED_BODY()
	
		
public:
	AMinigunBullet();
	virtual void BeginPlay() override;


};
