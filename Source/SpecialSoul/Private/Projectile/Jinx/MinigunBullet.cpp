// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Jinx/MinigunBullet.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Enemy/BaseEnemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Item/CBaseItem.h"

AMinigunBullet::AMinigunBullet()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Jinx/Projectiles/Attack/7_62_x_51_NATO_7_62_x_51_NATO_0.7_62_x_51_NATO_7_62_x_51_NATO_0'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
		MeshComp->SetCollisionProfileName("PlayerAttack");
		MeshComp->SetRelativeLocation(FVector(0.f, 60.f, 0.f));
		MeshComp->SetRelativeRotation(FRotator(-90, 0, 0));
	}

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempTailVfx(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/Jinx/VFX/Particles/Projectiles/NS_Jinx_Attack_Tail.NS_Jinx_Attack_Tail'"));
	if (TempTailVfx.Succeeded())
	{
		TailVfxAsset = TempTailVfx.Object;
		TailVfx->SetAsset(TailVfxAsset);
		TailVfx->bAutoActivate = true;
	}

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempHitVfx(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/Jinx/VFX/Particles/Projectiles/Hits/NS_Jinx_Attack_Hit.NS_Jinx_Attack_Hit'"));
	if (TempHitVfx.Succeeded())
	{
		HitVfxAsset = TempHitVfx.Object;
	}
	
}

void AMinigunBullet::BeginPlay()
{
	Super::BeginPlay();
	InitMoveComp();
	StartLocation  = GetActorLocation();
}

void AMinigunBullet::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
