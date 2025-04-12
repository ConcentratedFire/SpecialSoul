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

void UEnemyAnimInstance::AnimNotify_EnemyAttack()
{
	APawn* OwnerPawn = TryGetPawnOwner();
	if (!OwnerPawn || !ProjectileActor) return;
	
	FVector SpawnPoint = OwnerPawn->GetActorLocation();
	SpawnPoint += OwnerPawn->GetActorForwardVector() * 300;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerPawn;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GetWorld()->SpawnActor<AActor>(ProjectileActor, SpawnPoint, OwnerPawn->GetActorRotation(), SpawnParams);
}

void UEnemyAnimInstance::AnimNotify_DieEnd()
{
	ABaseEnemy* OwnerPawn = Cast<ABaseEnemy>(TryGetPawnOwner());
	if (!OwnerPawn) return;
	OwnerPawn->DieEndAction();
}
