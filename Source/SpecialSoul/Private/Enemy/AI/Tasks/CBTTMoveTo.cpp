// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Tasks/CBTTMoveTo.h"

#include "Enemy/BaseEnemy.h"

EBTNodeResult::Type UCBTTMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ABaseEnemy* MyEnemy = Cast<
		ABaseEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		AcceptableRadius = (MyEnemy->MoveDistance > 0.0f) ? MyEnemy->MoveDistance : DefaultAcceptableRadius;
	}
	else
		AcceptableRadius = DefaultAcceptableRadius;

	// 부모 클래스(UBTTask_MoveTo)의 ExecuteTask 호출
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
