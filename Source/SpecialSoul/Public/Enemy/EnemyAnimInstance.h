// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

class ABaseEnemy;
enum class EEnemyState : uint8;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float Speed { 0.f };


	 // FSM의 값을 BaseEnemy를 통해 받아온다 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	EEnemyState CurrentState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	bool bCanAttack { false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	bool bIsDead { false };

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ProjectileActor;

	UFUNCTION()
	void AnimNotify_EnemyAttack();

	UFUNCTION()
	void AnimNotify_DieEnd();
};
