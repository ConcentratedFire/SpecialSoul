// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Projectile.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/Jinx.h"

AProjectile::AProjectile()
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootSceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->SetAutoActivate(false);
	SetActorEnableCollision(true);

	TailVfx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TailVFX"));
	TailVfx->SetupAttachment(RootComponent);
	
	// HitVfx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ExplosionVFX"));
	// HitVfx->SetupAttachment(RootComponent);
	
	 // 이벤트 세팅
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::Hit);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	InitMoveComp();
	
	SetLifeTime();
}

void AProjectile::InitMoveComp()
{
	ProjectileMovementComp->InitialSpeed = 1500.f;
	ProjectileMovementComp->MaxSpeed = 1500.f;
	ProjectileMovementComp->ProjectileGravityScale = 0.f; // 직선운동
	ProjectileMovementComp->bRotationFollowsVelocity = true; // 추가 확인
	ProjectileMovementComp->Velocity = GetActorForwardVector() * ProjectileMovementComp->InitialSpeed;
	ProjectileMovementComp->Activate(true);
}

void AProjectile::SetLifeTime()
{ }

void AProjectile::Hit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HitVfxAsset)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVfxAsset, GetActorLocation());
	}
}

void AProjectile::ApplyCasterStat(ACBasePlayer* Caster)
{
	// TODO: BasePlayer의 공통 스탯을 this에게 적용하기!!
	// Caster에서 호출된다.
	if ( Cast<AJinx>(Caster))
		Damage = Cast<AJinx>(Caster)->AttackData.Damage;
}
