// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/Projectile.h"
#include "RocketLauncherBullet.generated.h"

class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ARocketLauncherBullet : public AProjectile
{
	GENERATED_BODY()
	
public:
	ARocketLauncherBullet();
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	float MoveAmplitude = 5.f; // 5cm

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	float MoveAmplitudeSpeed = 5.f; // 5cm

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	bool bMoveRight = true;
	
};
