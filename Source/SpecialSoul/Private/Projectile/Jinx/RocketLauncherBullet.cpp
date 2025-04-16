// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Jinx/RocketLauncherBullet.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"

ARocketLauncherBullet::ARocketLauncherBullet()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Jinx/Projectiles/E/red_big_space_ship.red_big_space_ship'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
		MeshComp->SetRelativeLocation(FVector(20.f, 0.f, 0.f));
		MeshComp->SetRelativeRotation(FRotator(-90.f, 0, 0));
		MeshComp->SetRelativeScale3D(FVector(0.2f));
	}
	MeshComp->SetCollisionProfileName("PlayerAttack");

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempTailVfx(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/Jinx/VFX/Particles/Projectiles/NS_Jinx_E_Tail.NS_Jinx_E_Tail'"));
	if (TempTailVfx.Succeeded())
	{
		TailVfxAsset = TempTailVfx.Object;
		TailVfx->SetAsset(TailVfxAsset);
		TailVfx->bAutoActivate = true;
	}

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempHitVfx(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/Jinx/VFX/Particles/Projectiles/Hits/NS_Jinx_E_Hit.NS_Jinx_E_Hit'"));
	if (TempHitVfx.Succeeded())
	{
		HitVfxAsset = TempHitVfx.Object;
	}
}

void ARocketLauncherBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ARocketLauncherBullet::ApplyLifeTime()
{
	// 사정거리 / 속도 = 수명 (초 단위)
	float LifeSpan = AttackRange / ProjectileMovementComp->InitialSpeed;
	SetLifeSpan(LifeSpan); // 수명 설정
}

void ARocketLauncherBullet::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 좌우 왔다갔다 하면서 날라감
		// Root기준으로 좌우 와리가리
	if (bMoveRight)
	{
		MeshComp->SetRelativeLocation(GetActorLocation() + GetActorRightVector() * MoveAmplitudeSpeed * DeltaSeconds);
		if (MeshComp->GetRelativeLocation().Y >= GetActorLocation().Y + MoveAmplitude)
			bMoveRight = false;
	}
	else
	{
		MeshComp->SetRelativeLocation(GetActorLocation() + -GetActorRightVector() * MoveAmplitudeSpeed * DeltaSeconds);
		if (MeshComp->GetRelativeLocation().Y <= GetActorLocation().Y - MoveAmplitude)
			bMoveRight = true;
	}
	
	// 360도 돌면서 날라감
	
}
