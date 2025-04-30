// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Enemy/RangedEnemyProjectile.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "ObjectPool/CObjectPoolManager.h"

ARangedEnemyProjectile::ARangedEnemyProjectile()
{
	bReplicates = true;
	SetReplicatingMovement(false); // 클라에선 코드로 위치 보간해주기
	SetNetUpdateFrequency(100.0f);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
		MeshComp->SetVisibility(false);
		if (HasAuthority())
		{
			MeshComp->SetCollisionProfileName("Enemy");
			MeshComp->SetNotifyRigidBodyCollision(true);
		}
		else
			MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		
		MeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap); // Player
		MeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore); // PlayerAttack
		MeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECR_Overlap); // Level
	}

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempTailVfx(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/RangeMinion/NS_RangedEnemy_Attack.NS_RangedEnemy_Attack'"));
	if (TempTailVfx.Succeeded())
	{
		TailVfxAsset = TempTailVfx.Object;
		TailVfx->SetAsset(TailVfxAsset);
		TailVfx->bAutoActivate = true;
		TailVfx->Activate();
		TailVfx->SetActive(true);
		// TailVfx->SetVisibility(false);
		// TailVfx->SetHiddenInGame(true);
		// TailVfx->Deactivate();
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

	AttackType = EAttackType::SingleTarget;
}


void ARangedEnemyProjectile::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARangedEnemyProjectile, ServerLocation);
}


void ARangedEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		ServerLocation = GetActorLocation();
	}
	else
	{
		// 클라이언트 보간 처리
			// 만약 120ms의 게임일 때
			// NetUpdateFrequency=100(즉, 100ms로 동기화처리)이면 동기화처리가 느림
			// 그렇기에 클라에서 추가로 보간해줘야함
		elapsedTime += DeltaTime;
		if (lastElapsedTime < KINDA_SMALL_NUMBER)
			return;
		
		FVector newLocation = ServerLocation + GetVelocity() * lastElapsedTime; 
		float lerpRatio = elapsedTime / lastElapsedTime;
		
		FVector lerpLocation = FMath::Lerp(ServerLocation, newLocation, lerpRatio);
		SetActorLocation(lerpLocation);

		// FVector TargetLocation = ServerLocation + GetVelocity() * lastElapsedTime;

		// InterpTo는 속도에 따라 보간해줌
		// FVector NewLocation = FMath::VInterpTo(ServerLocation, TargetLocation, lerpRatio, 10.f); // 10.f는 보간 속도
		// SetActorLocation(NewLocation);
	}
}

void ARangedEnemyProjectile::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);

	//if (!HasAuthority()) return;
	
	if (bNewHidden) // 풀 넣기 전 초기화
	{
		ProjectileMovementComp->Deactivate();
		ProjectileMovementComp->bSimulationEnabled = false;
		ProjectileMovementComp->StopMovementImmediately();
		
		TailVfx->Deactivate();
		TailVfx->SetActive(false);
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
		TailVfx->SetActive(true);
		TailVfx->SetVisibility(true);
		TailVfx->SetHiddenInGame(false);
	}
	//LOG_S(Error, TEXT("%d"), bNewHidden);
}

void ARangedEnemyProjectile::OnDestroy()
{
	if (!HasAuthority()) return;
	
	if (!ObjectPoolManager)
	{
		Destroy();
		return;
	}

	ObjectPoolManager->ReturnRangedEnemyProjectile(this);
}

void ARangedEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
		MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void ARangedEnemyProjectile::OnRep_ServerLocation()
{
	// 클라 영역
	//SetActorLocation(ServerLocation);

	lastElapsedTime = elapsedTime;

	elapsedTime = 0.0f;
}
