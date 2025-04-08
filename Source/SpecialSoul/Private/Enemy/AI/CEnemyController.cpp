// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/CEnemyController.h"

#include "Enemy/CMeleeEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

void ACEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	MyPawn = InPawn;
	
	FTimerHandle possessTimer;
	GetWorldTimerManager().SetTimer(possessTimer, [this, InPawn]()
	{		
		if (InPawn->IsA(ACMeleeEnemy::StaticClass()))
		{
			if (!BT_Enemy) return;

			RunBehaviorTree(BT_Enemy);
			BB_Enemy = GetBlackboardComponent();
			bUsingBT = true;
		}
	}, .2f, false);	
}

void ACEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bUsingBT)
	{
		BB_Enemy->SetValueAsObject(FName("Target"), TargetPlayer);
	}
}
