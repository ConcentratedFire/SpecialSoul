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

private:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* AttackComp;

	UFUNCTION()
	void OnMyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	float Damage{ 30.f }; // µ¥¹ÌÁö

public:
	void SetAttackCollision(bool bInEnable);
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
};
