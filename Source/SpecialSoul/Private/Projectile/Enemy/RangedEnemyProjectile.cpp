// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Enemy/RangedEnemyProjectile.h"

#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ARangedEnemyProjectile::ARangedEnemyProjectile()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
		MeshComp->SetVisibility(false);
		MeshComp->SetCollisionProfileName("Enemy");
	}

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempTailVfx(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/RangeMinion/NS_RangedEnemy_Attack.NS_RangedEnemy_Attack'"));
	if (TempTailVfx.Succeeded())
	{
		TailVfxAsset = TempTailVfx.Object;
		TailVfx->SetAsset(TailVfxAsset);
		TailVfx->bAutoActivate = true;
	}

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempHitVfx(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/Jinx/VFX/Particles/Projectiles/Hits/NS_RangedEnemy_Attack_Hit.NS_RangedEnemy_Attack_Hit'"));
	if (TempHitVfx.Succeeded())
	{
		HitVfxAsset = TempHitVfx.Object;
	}	
}

void ARangedEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ARangedEnemyProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARangedEnemyProjectile::ApplyLifeTime()
{
	float LifeSpan = AttackRange / ProjectileMovementComp->InitialSpeed;
	SetLifeSpan(LifeSpan); // 수명 설정
}

void ARangedEnemyProjectile::InitMoveComp()
{
	ProjectileMovementComp->InitialSpeed = 300.f;
	ProjectileMovementComp->MaxSpeed = 300.f;
	ProjectileMovementComp->ProjectileGravityScale = 0.f; // 직선운동
	ProjectileMovementComp->bRotationFollowsVelocity = true; // 추가 확인
	ProjectileMovementComp->Velocity = GetActorForwardVector() * ProjectileMovementComp->InitialSpeed;
	ProjectileMovementComp->bSimulationEnabled = true;
	ProjectileMovementComp->Activate(true);
}
