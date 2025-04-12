// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyAnimInstance.h"
#include "RangedEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API URangedEnemyAnimInstance : public UEnemyAnimInstance
{
	GENERATED_BODY()

public:
	// FSM의 값을 BaseEnemy를 통해 받아온다 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	EEnemyState CurrentState;

public: // 애님 노티파이
	virtual void AnimNotify_EnemyAttack() override;

	UFUNCTION()
	void AnimNotify_AttackEnd();
};
