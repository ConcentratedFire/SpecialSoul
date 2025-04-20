// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Services/CBTS_UpdateDistanceToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCBTS_UpdateDistanceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent();
	APawn* ownerPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (bb && ownerPawn)
	{
		AActor* Target = Cast<AActor>(bb->GetValueAsObject(TargetActorKey.SelectedKeyName));
		if (!Target || !Target->IsValidLowLevel())
		{
			bb->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, FLT_MAX);
			return;
		}

		float distance = FVector::Dist( Target->GetActorLocation(), ownerPawn->GetActorLocation() );
		bb->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, distance);
	}
}
