// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Decorators/CBTD_CheckTargetInSkillRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/MainBoss/MainBoss.h"
#include "Player/Components/SkillComponent.h"

UCBTD_CheckTargetInSkillRange::UCBTD_CheckTargetInSkillRange()
{
	DistanceToTarget.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UCBTD_CheckTargetInSkillRange, DistanceToTarget));

	// Blackboard 값 변경 감지 허용!
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
}

bool UCBTD_CheckTargetInSkillRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* aiController = OwnerComp.GetAIOwner();
	if (!aiController) return false;
	
	AMainBoss* boss = Cast<AMainBoss>(aiController->GetPawn());
	if (!boss) return false;

	UBlackboardComponent* blackboardComp = OwnerComp.GetBlackboardComponent();
	if (!blackboardComp) return false;
	
	USkillComponent* skillComp = boss->SkillComponent;
	if (!skillComp || !skillComp->SkillRangeMap.Contains(SkillKey)) return false;

	return blackboardComp->GetValueAsFloat(DistanceToTarget.SelectedKeyName) <=
		skillComp->SkillRangeMap[SkillKey];
}
