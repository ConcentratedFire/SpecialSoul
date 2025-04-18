// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Decorators/CBTCCheckPlayerInRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"

bool UCBTCCheckPlayerInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (ABaseEnemy* MyEnemy = Cast<
		ABaseEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		return MyEnemy->GetIsPlayerInRange((MyEnemy->MoveDistance > 0.0f) ? MyEnemy->MoveDistance : 100);
	}
	return Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
}
