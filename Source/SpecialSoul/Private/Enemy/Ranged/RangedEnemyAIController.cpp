// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Ranged/RangedEnemyAIController.h"

#include "EngineUtils.h"
#include "Enemy/Components/EnemyFSMComponent.h"
#include "Enemy/Ranged/RangedEnemy.h"
#include "PathFinding/FlowFieldActor.h"
#include "PathFinding/FlowFieldPFStrategy.h"
#include "PathFinding/NavMeshPFStrategy.h"

void ARangedEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	InitPFStrategy();
}

void ARangedEnemyAIController::InitPFStrategy()
{
	ARangedEnemy* RangedEnemy = Cast<ARangedEnemy>(MyPawn);
	if (!RangedEnemy) return;
	
	if (!FlowField/*!FindFlowField()*/)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Flow Field Actor"));
		return;
	}
	
	UEnemyFSMComponent* FSM = RangedEnemy->FindComponentByClass<UEnemyFSMComponent>();
	if (!FSM) return;
	
	if (bUseFlowField) // Custom FlowField
	{
		FFStrategy->Initialize(FlowField, RangedEnemy); // PF전략이 폰(RangedEnemy) 제어 
		FSM->InitPathFindingStrategy(FFStrategy); // FSM이 PF전략을 사용할 수 있도록 DI
	}
	else // NavMeshAI
	{
		UNavMeshPFStrategy* NavStrategy = NewObject<UNavMeshPFStrategy>(this);
		NavStrategy->Initialize(RangedEnemy); 
		FSM->InitPathFindingStrategy(NavStrategy);
	}
}

