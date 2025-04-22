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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	void SetManager(class ACObjectPoolManager* Manager) { ObjectPoolManager = Manager; }

protected:
	virtual void OnDestroy() override;
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TObjectPtr<ACObjectPoolManager> ObjectPoolManager;
	
	UPROPERTY(ReplicatedUsing = OnRep_ServerLocation)
	FVector ServerLocation;

	UFUNCTION()
	void OnRep_ServerLocation();

private:
	float elapsedTime = 0.0f;
	float lastElapsedTime = 0.0f;


public:
	// UFUNCTION(NetMulticast, Unreliable)
	// void MRPC_Move();
	
};
