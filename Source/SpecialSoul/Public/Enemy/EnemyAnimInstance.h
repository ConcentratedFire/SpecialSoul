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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<ABaseEnemy> OwnerEnemy;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float Speed { 0.f };

protected:
	UFUNCTION()
	virtual void AnimNotify_EnemyAttack();

	UFUNCTION()
	virtual void AnimNotify_DieEnd();

	UFUNCTION()
	virtual void AnimNotify_BeginMeleeAttack();

	UFUNCTION()
	virtual void AnimNotify_EndMeleeAttack();
};
