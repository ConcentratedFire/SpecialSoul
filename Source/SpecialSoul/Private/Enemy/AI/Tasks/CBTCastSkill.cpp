// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Tasks/CBTCastSkill.h"

#include "AIController.h"
#include "Enemy/MainBoss/MainBoss.h"
#include "Player/Components/SkillComponent.h"

EBTNodeResult::Type UCBTCastSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMainBoss* mainBoss = Cast<AMainBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (mainBoss && mainBoss->SkillComponent)
	{
		mainBoss->SkillComponent->CastSkill(SkillKey);
		return Super::ExecuteTask(OwnerComp, NodeMemory);
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
