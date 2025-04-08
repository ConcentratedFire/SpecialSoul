// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Tasks/CBTTAttack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/AI/CEnemyController.h"

EBTNodeResult::Type UCBTTAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ABaseEnemy* MyEnemy = Cast<
		ABaseEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		MyEnemy->HandleAttack();
		bNotifyTick = true;
		return EBTNodeResult::InProgress;
	}
	else
		return EBTNodeResult::Failed;
}

void UCBTTAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (ABaseEnemy* MyEnemy = Cast<
		ABaseEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("EndAttack")))
		{
			if (auto EnemyController = Cast<ACEnemyController>(MyEnemy->Controller))
				EnemyController->bEndAttack = false;
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
