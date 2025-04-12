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
	
	if (!FindFlowField())
	{
		UE_LOG(LogTemp, Warning, TEXT("No Flow Field Actor"));
		return;
	}
	
	//if (bUsePathFindingStrategy)
	//{
	UEnemyFSMComponent* FSM = RangedEnemy->FindComponentByClass<UEnemyFSMComponent>();
	if (!FSM) return;
	
	if (bUseFlowField) // Custom FlowField
	{
		UFlowFieldPFStrategy* FFStrategy = NewObject<UFlowFieldPFStrategy>(this);
		FFStrategy->Initialize(RangedEnemy, FlowField); // PF전략이 폰(RangedEnemy) 제어 
		FSM->InitPathFindingStrategy(FFStrategy); // FSM이 PF전략을 사용할 수 있도록 DI
	}
	else // NavMeshAI
	{
		UNavMeshPFStrategy* NavStrategy = NewObject<UNavMeshPFStrategy>(this);
		NavStrategy->Initialize(RangedEnemy); 
		FSM->InitPathFindingStrategy(NavStrategy);
	}
//}
}

bool ARangedEnemyAIController::FindFlowField()
{
	for (TActorIterator<AFlowFieldActor> It(GetWorld(), AFlowFieldActor::StaticClass()); It; ++It)
	{
		FlowField = *It;
		return true;
	}
	return false;
}

