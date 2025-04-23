// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyAnimInstance.h"
#include "MainBossAnimInstance.generated.h"

class AMainBoss;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UMainBossAnimInstance : public UEnemyAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<AMainBoss> OwnerMainBoss;

	UPROPERTY(VisibleAnywhere, Category = "MainBoss")
	bool bIsUlt {false}; // 궁 쓴 상태

protected:
	virtual void AnimNotify_EnemyAttack() override;

	virtual void AnimNotify_DieEnd() override;
};
