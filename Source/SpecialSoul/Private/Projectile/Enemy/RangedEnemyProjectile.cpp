// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Enemy/RangedEnemyProjectile.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ObjectPool/CObjectPoolManager.h"

ARangedEnemyProjectile::ARangedEnemyProjectile()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
		MeshComp->SetVisibility(false);
		MeshComp->SetCollisionProfileName("Enemy");
		// MeshComp->SetCollisionResponseToChannel(, ECR_Ignore);
	}

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempTailVfx(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/RangeMinion/NS_RangedEnemy_Attack.NS_RangedEnemy_Attack'"));
	if (TempTailVfx.Succeeded())
	{
		TailVfxAsset = TempTailVfx.Object;
		TailVfx->SetAsset(TailVfxAsset);
		TailVfx->bAutoActivate = false;
		TailVfx->SetVisibility(false);
		TailVfx->SetHiddenInGame(true);
		TailVfx->SetActive(false);
	}

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempHitVfx(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/Jinx/VFX/Particles/Projectiles/Hits/NS_RangedEnemy_Attack_Hit.NS_RangedEnemy_Attack_Hit'"));
	if (TempHitVfx.Succeeded())
	{
		HitVfxAsset = TempHitVfx.Object;
	}

	if (ProjectileMovementComp)
	{
		// 초기값 설정
		ProjectileMovementComp->SetAutoActivate(false); // 풀을 사용해야하기에 자동 실행 꺼두기!

		ProjectileMovementComp->InitialSpeed = 300.f;
		ProjectileMovementComp->MaxSpeed = 300.f;
		ProjectileMovementComp->ProjectileGravityScale = 0.f;
		ProjectileMovementComp->bRotationFollowsVelocity = true;
		ProjectileMovementComp->bSimulationEnabled = true;
	}
}

void ARangedEnemyProjectile::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	if (bNewHidden) // 풀 넣기 전 초기화
	{
		ProjectileMovementComp->Deactivate();
		ProjectileMovementComp->bSimulationEnabled = false;
		ProjectileMovementComp->StopMovementImmediately();
		
		TailVfx->Deactivate();
		TailVfx->SetVisibility(false);
		TailVfx->SetHiddenInGame(true);
	}
	else //풀에서 나올 때
	{
		StartLocation = GetActorLocation();

		ProjectileMovementComp->Activate();
		ProjectileMovementComp->bSimulationEnabled = true;
		ProjectileMovementComp->Velocity = GetActorForwardVector() * ProjectileMovementComp->InitialSpeed;

		TailVfx->Activate();
		TailVfx->SetVisibility(true);
		TailVfx->SetHiddenInGame(false);
	}
}

void ARangedEnemyProjectile::OnDestroy()
{
	if (!ObjectPoolManager)
	{
		Destroy();
		return;
	}

	ObjectPoolManager->ReturnRangedEnemyProjectile(this);
}