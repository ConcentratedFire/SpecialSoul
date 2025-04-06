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
	
	APawn* OwnerPawn = TryGetPawnOwner();
	if (!OwnerPawn) return;
	
	Speed = OwnerPawn->GetVelocity().Size();
}
