// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Services/CBTSPlayerInRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"

void UCBTSPlayerInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	// UE_LOG(LogTemp, Display, TEXT("Player In Range"));

	if (ABaseEnemy* MyEnemy = Cast<ABaseEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		bool bIsInRange = MyEnemy->GetIsPlayerInRange((MyEnemy->MoveDistance > 0.0f) ? MyEnemy->MoveDistance : 100);
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), bIsInRange ? 1 : 0);
	}
}
