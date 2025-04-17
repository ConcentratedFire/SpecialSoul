// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTMoveToDirection.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCBTTMoveToDirection : public UBTTaskNode
{
	GENERATED_BODY()

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 600.0f; // 이동 속도
};
