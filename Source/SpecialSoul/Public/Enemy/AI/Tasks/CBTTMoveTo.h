// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "CBTTMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCBTTMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "MoveTo")
	float DefaultAcceptableRadius = 100.0f;
};
