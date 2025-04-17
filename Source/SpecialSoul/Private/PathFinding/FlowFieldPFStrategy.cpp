// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinding/FlowFieldPFStrategy.h"

#include "GameFramework/Character.h"
#include "PathFinding/FlowFieldActor.h"


void UFlowFieldPFStrategy::Initialize(AFlowFieldActor* InFlowField, ACharacter* InCharacter)
{
	FlowField = InFlowField;
	
	if (InCharacter)
		ControlledCharacter = InCharacter;
}

void UFlowFieldPFStrategy::MoveTo(const FVector& InTargetLocation)
{
	TargetLocation = InTargetLocation;
}

void UFlowFieldPFStrategy::TickMove(float DeltaTime)
{
	if (!ControlledCharacter) return;
	
	FVector2D MoveDir = GetFlowFieldDirection(ControlledCharacter->GetActorLocation());
	ControlledCharacter->AddMovementInput(FVector(MoveDir.X, MoveDir.Y, 0.f));
}

FVector2D UFlowFieldPFStrategy::GetFlowFieldDirection(const FVector& InLocation)
{
	return FlowField->FindFlowDirection(InLocation);
}
