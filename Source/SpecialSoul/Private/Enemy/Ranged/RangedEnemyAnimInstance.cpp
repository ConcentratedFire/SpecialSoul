// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Ranged/RangedEnemyAnimInstance.h"

#include "Components/CapsuleComponent.h"
#include "Enemy/Ranged/RangedEnemy.h"
#include "ObjectPool/CObjectPoolManager.h"
#include "Projectile/Enemy/RangedEnemyProjectile.h"

void URangedEnemyAnimInstance::AnimNotify_EnemyAttack()
{
	if (!TryGetPawnOwner()->HasAuthority())
	{
		return;
	}
	
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
	SpawnPoint += RangedEnemy->GetActorForwardVector() * 30;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = RangedEnemy;
	SpawnParams.Instigator = RangedEnemy;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	FTransform spawnTransform;
	spawnTransform.SetLocation(SpawnPoint);
	spawnTransform.SetRotation(OwnerEnemy->GetActorRotation().Quaternion());
	
	RangedEnemy->ObjectPoolManager->RangedEnemyProjectileSpawn(spawnTransform);
}

void URangedEnemyAnimInstance::AnimNotify_AttackEnd()
{
	OwnerEnemy->bIsAttacking = false;
}
