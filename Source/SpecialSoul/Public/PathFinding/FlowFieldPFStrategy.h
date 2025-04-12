// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/PathFindingStrategy.h"
#include "UObject/NoExportTypes.h"
#include "FlowFieldPFStrategy.generated.h"

class AFlowFieldActor;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UFlowFieldPFStrategy : public UObject, public IPathFindingStrategy
{
	GENERATED_BODY()

public:
	virtual void MoveTo(const FVector& InTargetLocation) override;
	virtual void TickMove(float DeltaTime) override;

	void Initialize(ACharacter* InCharacter, AFlowFieldActor* InFlowField);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PathFinding)
	ACharacter* ControlledCharacter;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<AFlowFieldActor> FlowField;

	FVector TargetLocation;
	
};
