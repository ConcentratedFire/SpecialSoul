// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/PathFindingStrategy.h"
#include "UObject/NoExportTypes.h"
#include "NavMeshPFStrategy.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UNavMeshPFStrategy : public UObject, public IPathFindingStrategy
{
	GENERATED_BODY()

public:
	void Initialize(ACharacter* Character);
	
	virtual void MoveTo(const FVector& TargetLocation) override;
	virtual void TickMove(float DeltaTime) override;

};
