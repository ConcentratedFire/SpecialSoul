// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAnimInstance.h"

#include "Enemy/BaseEnemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	auto OwnerPawn = TryGetPawnOwner();
	if (!OwnerPawn) return;
	OwnerEnemy = Cast<ABaseEnemy>(OwnerPawn);
	if (!OwnerEnemy) return;

	Speed = OwnerEnemy->GetVelocity().Size();
}

void UEnemyAnimInstance::AnimNotify_EnemyAttack()
{
	auto OwnerPawn = TryGetPawnOwner();
	if (!OwnerPawn) return;
	OwnerEnemy = Cast<ABaseEnemy>(OwnerPawn);

	if (!OwnerEnemy || !ProjectileActor) return;
}

void UEnemyAnimInstance::AnimNotify_DieEnd()
{
	ABaseEnemy* OwnerPawn = Cast<ABaseEnemy>(TryGetPawnOwner());
	if (!OwnerPawn || !OwnerPawn->HasAuthority())return;
	OwnerPawn->DieEndAction();
}
