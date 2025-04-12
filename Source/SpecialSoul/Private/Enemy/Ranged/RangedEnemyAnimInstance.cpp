// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Ranged/RangedEnemyAnimInstance.h"

#include "Enemy/Ranged/RangedEnemy.h"
#include "Projectile/Projectile.h"

void URangedEnemyAnimInstance::AnimNotify_EnemyAttack()
{
	ARangedEnemy* RangedEnemy = Cast<ARangedEnemy>(TryGetPawnOwner());
	if (!RangedEnemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerEnemy is nulltpr..."));
		return;
	}
	
	if (!RangedEnemy->ProjectileActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerEnemy->ProjectileActor is nulltpr..."));
		return;
	}
	
	// 투사체 발사
	FVector SpawnPoint = RangedEnemy->GetActorLocation();
	SpawnPoint += RangedEnemy->GetActorForwardVector() * 300;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = RangedEnemy;
	SpawnParams.Instigator = RangedEnemy;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GetWorld()->SpawnActor<AProjectile>(RangedEnemy->ProjectileActor, SpawnPoint, OwnerEnemy->GetActorRotation(), SpawnParams);
}

void URangedEnemyAnimInstance::AnimNotify_AttackEnd()
{
	//UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackEnd"));
	OwnerEnemy->bIsAttacking = false;
}
