// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Decorators/CBTD_CanUseSkill.h"

#include "AIController.h"
#include "Enemy/MainBoss/MainBoss.h"
#include "Player/Components/SkillComponent.h"

bool UCBTD_CanUseSkill::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AMainBoss* mb = Cast<AMainBoss>(OwnerComp.GetAIOwner()->GetPawn());

	if (mb && mb->SkillComponent)
	{
		return mb->SkillComponent->CanUseSkill(SkillKey);
	}
	
	return false;

}
