// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTS_UpdateDistanceToTarget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCBTS_UpdateDistanceToTarget : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = My)
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = My)
	FBlackboardKeySelector DistanceToTargetKey;
};
