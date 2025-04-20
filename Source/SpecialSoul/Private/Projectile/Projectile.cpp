// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Projectile.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/Jinx.h"

AProjectile::AProjectile()
{
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
	
	 // 이벤트 세팅
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::Hit);
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
	ProjectileMovementComp->InitialSpeed = 1500.f;
	ProjectileMovementComp->MaxSpeed = 1500.f;
	ProjectileMovementComp->ProjectileGravityScale = 0.f; // 직선운동
	ProjectileMovementComp->bRotationFollowsVelocity = true; // 추가 확인
	ProjectileMovementComp->Velocity = GetActorForwardVector() * ProjectileMovementComp->InitialSpeed;
	ProjectileMovementComp->bSimulationEnabled = true;
	ProjectileMovementComp->Activate(true);
}

void AProjectile::Hit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HitVfxAsset)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVfxAsset, GetActorLocation());
	}
	Penetration--;
	if (Penetration <= 0)
	{
		OnDestroy();
	}
}

void AProjectile::OnDestroy()
{
	Destroy();
}

void AProjectile::ApplyCasterStat(ACharacter* Caster)
{
	// TODO: BasePlayer의 공통 스탯을 this에게 적용하기!!
	// Caster에서 호출된다.
	if ( Cast<AJinx>(Caster))
		Damage = Cast<AJinx>(Caster)->JinxAttackData.Damage;
}
