// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/AI/CEnemyController.h"
#include "RangedEnemyAIController.generated.h"

class ACBasePlayer;
class AFlowFieldActor;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ARangedEnemyAIController : public ACEnemyController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bUseFlowField {true};

	UPROPERTY()
	TObjectPtr<AFlowFieldActor> FlowField;

	void InitPFStrategy();
	bool FindFlowField();
};
