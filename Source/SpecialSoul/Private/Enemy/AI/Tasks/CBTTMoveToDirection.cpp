// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Tasks/CBTTMoveToDirection.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"

EBTNodeResult::Type UCBTTMoveToDirection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ABaseEnemy* MyEnemy = Cast<
		ABaseEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		FVector Direction = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("MoveDir"));
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), MyEnemy->GetActorLocation()+Direction*Speed);
		// MyEnemy->AddMovementInput(Direction.GetSafeNormal() * Speed);
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
