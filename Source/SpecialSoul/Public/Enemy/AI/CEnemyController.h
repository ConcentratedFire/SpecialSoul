// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ACEnemyController : public AAIController
{
	GENERATED_BODY()

private:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AActor* TargetPlayer;
	
private:	// Behavior Tree, For Melee Enemy & Middle Boss
	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* BT_Enemy;
	UPROPERTY()
	class UBlackboardComponent* BB_Enemy;
	bool bUsingBT;

private:
	UPROPERTY()
	class APawn* MyPawn;
};
