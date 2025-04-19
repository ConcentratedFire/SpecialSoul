// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTD_CanUseSkill.generated.h"

enum class ESkillKey : uint8;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCBTD_CanUseSkill : public UBTDecorator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = My)
	ESkillKey SkillKey;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
