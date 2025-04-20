// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MainBoss/MainBossController.h"

#include "BehaviorTree/BlackboardComponent.h"

void AMainBossController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AMainBossController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bUsingBT)
	{
		// BB_Enemy->SetValue ~
	}
}
