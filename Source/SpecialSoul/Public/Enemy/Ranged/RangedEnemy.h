// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "RangedEnemy.generated.h"

class ARangedEnemyProjectile;
class AProjectile;
class IPathFindingStrategy;
class AFlowFieldActor;
/**
 * 
 */

UCLASS()
class SPECIALSOUL_API ARangedEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	ARangedEnemy();
	virtual void Tick(float DeltaTime) override;

	virtual void DieEndAction() override;
protected:
	virtual void BeginPlay() override;

	virtual void FindTarget() override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FSM")
	TObjectPtr<UEnemyFSMComponent> FSMComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FSM")
	TSubclassOf<ARangedEnemyProjectile> ProjectileActor;
};
