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
	virtual void Tick(float DeltaSeconds) override;
protected:
	//virtual void ApplyLifeTime() override;
	
	// UFUNCTION()
	// void OnMeshCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	//                             const FHitResult& SweepResult);

	
};
