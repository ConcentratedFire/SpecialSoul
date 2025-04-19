// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "CBTD_CheckTargetInSkillRange.generated.h"

enum class ESkillKey : uint8;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCBTD_CheckTargetInSkillRange : public UBTDecorator_Blackboard
{
	GENERATED_BODY()

public:
	UCBTD_CheckTargetInSkillRange();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	UPROPERTY(EditAnywhere, Category = My)
	FBlackboardKeySelector DistanceToTarget;
	
	UPROPERTY(EditAnywhere, Category = My)
	ESkillKey SkillKey;

};
