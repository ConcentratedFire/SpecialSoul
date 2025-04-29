// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAnimInstance.h"

#include "Enemy/BaseEnemy.h"
#include "Enemy/CMeleeEnemy.h"
#include "Enemy/CMiddleBoss.h"

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
	ACMiddleBoss* OwnerPawn = Cast<ACMiddleBoss>(TryGetPawnOwner());
	if (OwnerPawn && OwnerPawn->HasAuthority())
		OwnerPawn->SRPC_Attack();
}

void UEnemyAnimInstance::AnimNotify_DieEnd()
{
	ABaseEnemy* OwnerPawn = Cast<ABaseEnemy>(TryGetPawnOwner());
	if (OwnerPawn && OwnerPawn->HasAuthority())
		OwnerPawn->DieEndAction();
}

void UEnemyAnimInstance::AnimNotify_BeginMeleeAttack()
{
	ACMeleeEnemy* OwnerPawn = Cast<ACMeleeEnemy>(TryGetPawnOwner());
	if (OwnerPawn && OwnerPawn->HasAuthority())
		OwnerPawn->SetAttackCollision(true);
}

void UEnemyAnimInstance::AnimNotify_EndMeleeAttack()
{
	ACMeleeEnemy* OwnerPawn = Cast<ACMeleeEnemy>(TryGetPawnOwner());
	if (OwnerPawn && OwnerPawn->HasAuthority())
		OwnerPawn->SetAttackCollision(false);
}
