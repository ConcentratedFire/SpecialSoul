// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MainBoss/MainBossAnimInstance.h"

#include "Enemy/MainBoss/MainBoss.h"

void UMainBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMainBossAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	OwnerMainBoss = Cast<AMainBoss>(TryGetPawnOwner());
	bIsUlt = OwnerMainBoss->bIsUlt;
}

void UMainBossAnimInstance::AnimNotify_EnemyAttack()
{
	OwnerMainBoss = Cast<AMainBoss>(TryGetPawnOwner());

	if (!OwnerMainBoss) return;
}

void UMainBossAnimInstance::AnimNotify_DieEnd()
{
	ABaseEnemy* OwnerPawn = Cast<ABaseEnemy>(TryGetPawnOwner());
	if (!OwnerPawn) return;
	OwnerPawn->DieEndAction();
}
