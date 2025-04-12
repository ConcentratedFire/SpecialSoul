// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinding/FlowFieldPFStrategy.h"

#include "GameFramework/Character.h"
#include "PathFinding/FlowFieldActor.h"


void UFlowFieldPFStrategy::Initialize(ACharacter* InCharacter, AFlowFieldActor* InFlowField)
{
	ControlledCharacter = InCharacter;
	FlowField = InFlowField;
}


void UFlowFieldPFStrategy::MoveTo(const FVector& InTargetLocation)
{
	TargetLocation = InTargetLocation;
}

void UFlowFieldPFStrategy::TickMove(float DeltaTime)
{
	FVector2D MoveDir = FlowField->FindFlowDirection(ControlledCharacter->GetActorLocation());
	ControlledCharacter->AddMovementInput(FVector(MoveDir.X, MoveDir.Y, 0.f));
}

