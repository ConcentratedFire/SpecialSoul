// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MainBoss/MainBossController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/MainBoss/MainBoss.h"

void AMainBossController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetActorTickEnabled(true);
	RunBehaviorTree(BT_Enemy);
}

void AMainBossController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bUsingBT)
	{
		// BB_Enemy->SetValue ~
	}
}

void AMainBossController::SetActorTickEnabled(bool bEnabled)
{
	Super::SetActorTickEnabled(bEnabled);
	
	if (MyPawn->IsA(AMainBoss::StaticClass()))
	{
		RunBehaviorTree(bEnabled ? BT_Enemy : nullptr);
		bUsingBT = bEnabled;

		if (bEnabled)
			BB_Enemy = GetBlackboardComponent();
	}
}
