// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Jinx/MegaRocketBullet.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"

// 360 돌면서 나간다
AMegaRocketBullet::AMegaRocketBullet()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Jinx/Projectiles/R/SM_rocket_low.SM_rocket_low'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
		MeshComp->SetRelativeLocation(FVector(60.f, 0.f, 50.f));
		MeshComp->SetRelativeRotation(FRotator(180.f, 0, 0));
		MeshComp->SetRelativeScale3D(FVector(2.5f));
		MeshComp->SetCollisionProfileName("PlayerAttack");
	}

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempTailVfx(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/Jinx/VFX/Particles/Fires/NS_Jinx_R_Tail.NS_Jinx_R_Tail'"));
	if (TempTailVfx.Succeeded())
	{
		TailVfxAsset = TempTailVfx.Object;
		TailVfx->SetAsset(TailVfxAsset);
		TailVfx->SetRelativeLocation(FVector(-20.f, 0.f, 10.f));
		TailVfx->SetRelativeRotation(FRotator(90, 0, 0));
		TailVfx->SetRelativeScale3D(FVector(0.1f, 0.1f, 1.f));
		TailVfx->bAutoActivate = true;
	}

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempHitVfx(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/Jinx/VFX/Particles/Projectiles/Hits/NS_Jinx_R_Hit.NS_Jinx_R_Hit'"));
	if (TempHitVfx.Succeeded())
	{
		HitVfxAsset = TempHitVfx.Object;
	}

	AttackType = EAttackType::AreaOfEffect;
	ExplosionRadius = 250.f;
}

void AMegaRocketBullet::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
		MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	else
	{
		InitMoveComp();
		StartLocation  = GetActorLocation();
	}
}


void AMegaRocketBullet::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 매우 작게 시작해서, 점차 커짐
	
	// 360도 돌면서 날아감
}
