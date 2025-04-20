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

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void SetActorTickEnabled(bool bEnabled) override;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AActor* TargetPlayer;
	UPROPERTY(Replicated)
	bool bEndAttack;
	
protected:	// Behavior Tree, For Melee Enemy & Middle Boss
	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* BT_Enemy;
	UPROPERTY()
	class UBlackboardComponent* BB_Enemy;
	bool bUsingBT;


protected:
	UPROPERTY()
	class APawn* MyPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bUseFlowField {true};

	UPROPERTY()
	TObjectPtr<class AFlowFieldActor> FlowField;

	bool FindFlowField();

	UPROPERTY()
	class UFlowFieldPFStrategy* FFStrategy;
};
