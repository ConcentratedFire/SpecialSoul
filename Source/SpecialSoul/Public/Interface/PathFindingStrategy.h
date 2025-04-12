// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PathFindingStrategy.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPathFindingStrategy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPECIALSOUL_API IPathFindingStrategy
{
	GENERATED_BODY()

public:

	virtual void MoveTo(const FVector& TargetLocation) = 0;
	virtual void TickMove(float DeltaTime) = 0;

};
