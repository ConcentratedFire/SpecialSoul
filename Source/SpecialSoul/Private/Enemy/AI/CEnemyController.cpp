// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/CEnemyController.h"

#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/CMeleeEnemy.h"
#include "Enemy/CMiddleBoss.h"
#include "Enemy/MainBoss/MainBoss.h"
#include "Net/UnrealNetwork.h"
#include "PathFinding/FlowFieldActor.h"
#include "PathFinding/FlowFieldPFStrategy.h"

void ACEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	MyPawn = InPawn;

	// FTimerHandle possessTimer;
	// GetWorldTimerManager().SetTimer(possessTimer, [this, InPawn]()
	// {
	// 	// if (InPawn->IsA(ACMeleeEnemy::StaticClass()) || InPawn->IsA(ACMiddleBoss::StaticClass())
	// 	// 	|| InPawn->IsA(AMainBoss::StaticClass()))
	// 	if (InPawn->IsA(ABaseEnemy::StaticClass()))
	// 	{
	// 		if (!BT_Enemy) return;
	//
	// 		RunBehaviorTree(BT_Enemy);
	// 		BB_Enemy = GetBlackboardComponent();
	// 		bUsingBT = true;
	// 	}
	// }, .2f, false);

	FindFlowField();

	if (bUseFlowField) // Custom FlowField
	{
		FFStrategy = NewObject<UFlowFieldPFStrategy>(this);
		FFStrategy->Initialize(FlowField); // PF전략이 폰(RangedEnemy) 제어 
	}
}

void ACEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bUsingBT)
	{
		BB_Enemy->SetValueAsBool(FName("UseFlowField"), bUseFlowField);
		BB_Enemy->SetValueAsObject(FName("Target"), TargetPlayer);
		if (bEndAttack)
			BB_Enemy->SetValueAsBool(FName("EndAttack"), bEndAttack);

		if (bUseFlowField)
		{
			auto MoveDir = FFStrategy->GetFlowFieldDirection(MyPawn->GetActorLocation());
			FVector TargetLocation = MyPawn->GetActorLocation() + FVector(MoveDir.X, MoveDir.Y, 0) * 50;
			BB_Enemy->SetValueAsVector(FName("TargetLocation"), TargetLocation);
			BB_Enemy->SetValueAsVector(FName("MoveDir"), FVector(MoveDir.X, MoveDir.Y, 0));
		}
	}
}

void ACEnemyController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACEnemyController, bEndAttack);
}

void ACEnemyController::SetActorTickEnabled(bool bEnabled)
{
	Super::SetActorTickEnabled(bEnabled);

	if (MyPawn->IsA(ACMeleeEnemy::StaticClass()) || MyPawn->IsA(ACMiddleBoss::StaticClass()))
	{
		RunBehaviorTree(bEnabled ? BT_Enemy : nullptr);
		bUsingBT = bEnabled;

		if (bEnabled)
			BB_Enemy = GetBlackboardComponent();
	}
}

bool ACEnemyController::FindFlowField()
{
	for (TActorIterator<AFlowFieldActor> It(GetWorld(), AFlowFieldActor::StaticClass()); It; ++It)
	{
		FlowField = *It;
		return true;
	}
	return false;
}
