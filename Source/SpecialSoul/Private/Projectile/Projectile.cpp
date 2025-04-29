// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Projectile.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Enemy/BaseEnemy.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Item/CBaseItem.h"
#include "Player/Jinx.h"

AProjectile::AProjectile()
{
	bReplicates = true;
	SetReplicatingMovement(true); 
	
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootSceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->SetAutoActivate(false);
	SetActorEnableCollision(true);

	TailVfx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TailVFX"));
	TailVfx->SetupAttachment(RootComponent);
	
	//  이벤트 세팅
	if (HasAuthority())
	{
		MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);
	}
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurLocation = GetActorLocation();
	CurrentDistance = FVector::Dist(StartLocation, CurLocation);
	if (CurrentDistance >= AttackRange)
	{
		OnDestroy(); // 제거
	}
}

void AProjectile::InitMoveComp()
{
	//LOG_S(Warning, TEXT("AProjectile::InitMoveComp"));
	ProjectileMovementComp->InitialSpeed = 1500.f;
	ProjectileMovementComp->MaxSpeed = 1500.f;
	ProjectileMovementComp->ProjectileGravityScale = 0.f; // 직선운동
	ProjectileMovementComp->bRotationFollowsVelocity = true; // 추가 확인
	ProjectileMovementComp->Velocity = GetActorForwardVector() * ProjectileMovementComp->InitialSpeed;
	ProjectileMovementComp->bSimulationEnabled = true;
	ProjectileMovementComp->Activate(true);
}

void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HitVfxAsset)
	{
		MRPC_SpawnHitVFX();
	}

	if (AttackType == EAttackType::SingleTarget)
	{
		if (auto Enemy = Cast<ABaseEnemy>(OtherActor))
		{
			Enemy->MyDamage(Damage);
		}
		else if (auto Item = Cast<ACBaseItem>(OtherActor))
		{
			if (Item->GetActorNameOrLabel().Contains("ItemBox"))
				Item->ActiveItem();
		}
		else if (auto player = Cast<ACBasePlayer>(OtherActor))
		{
			player->DamageProcess(Damage);
		}
	}
	else // 범위공격 AOE
	{
		
		TArray<FHitResult> hitResults;
		FCollisionShape SphereShape = FCollisionShape::MakeSphere(ExplosionRadius);
		bool bHit = GetWorld()->SweepMultiByChannel(hitResults, GetActorLocation(), GetActorLocation(),
			FQuat::Identity, MeshComp->GetCollisionObjectType(), SphereShape);

		for (FHitResult hitResult : hitResults)
		{
			//UE_LOG(LogTemp, Error, TEXT("hitResults.Num() = %d"), hitResults.Num());
			FVector hitLocation = hitResult.ImpactPoint; // 충돌지점

			// DrawDebugSphere(GetWorld(), hitLocation, ExplosionRadius, 20, FColor::Yellow, false, 0.1f);
			
			AActor* hitActor = hitResult.GetActor();
			if (auto Enemy = Cast<ABaseEnemy>(hitActor))
			{
				Enemy->MyDamage(Damage);
			}
			else if (auto Item = Cast<ACBaseItem>(hitActor))
			{
				if (Item->GetActorNameOrLabel().Contains("ItemBox"))
					Item->ActiveItem();
			}
			else if (auto player = Cast<ACBasePlayer>(hitActor))
			{
				player->DamageProcess(Damage);
			}
		}
	}
	
	
	Penetration--;
	if (Penetration <= 0)
	{
		OnDestroy();
	}
}

void AProjectile::MRPC_SpawnHitVFX_Implementation()
{
	if (HitVfxAsset)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVfxAsset, GetActorLocation());
	}
}

void AProjectile::OnDestroy()
{
	if (HasAuthority())
	{
		Destroy();
	}
}

void AProjectile::ApplyCasterStat(ACharacter* Caster)
{
	if (auto jinx = Cast<AJinx>(Caster))
	{
		bool bIsCri;
		Damage = jinx->GetDamage(bIsCri);
	}
}
