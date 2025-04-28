// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MainBoss/MainBossAnimInstance.h"

#include "SpecialSoul.h"
#include "Components/BoxComponent.h"
#include "Enemy/MainBoss/MainBoss.h"

void UMainBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMainBossAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	OwnerMainBoss = Cast<AMainBoss>(TryGetPawnOwner());
	if (OwnerMainBoss)
		bIsUlt = OwnerMainBoss->bIsUlt;
}

void UMainBossAnimInstance::AnimNotify_BeginAttack()
{
	OwnerMainBoss = Cast<AMainBoss>(TryGetPawnOwner());
	if (!OwnerMainBoss) return;
	if (!OwnerMainBoss->HasAuthority()) return;

	OwnerMainBoss->BladeHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//LOG_S(Log, TEXT("UMainBossAnimInstance::AnimNotify_BeginAttack"));
}

void UMainBossAnimInstance::AnimNotify_EndAttack()
{
	OwnerMainBoss = Cast<AMainBoss>(TryGetPawnOwner());
	if (!OwnerMainBoss) return;
	if (!OwnerMainBoss->HasAuthority()) return;

	OwnerMainBoss->BladeHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//LOG_S(Log, TEXT("UMainBossAnimInstance::AnimNotify_EndAttack"));
}

void UMainBossAnimInstance::AnimNotify_DieEnd()
{
	ABaseEnemy* OwnerPawn = Cast<ABaseEnemy>(TryGetPawnOwner());
	if (!OwnerPawn) return;
	OwnerPawn->DieEndAction();
}
