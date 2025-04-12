// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

AProjectile::AProjectile()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 1500.f;
	ProjectileMovementComp->MaxSpeed = 1500.f;
	ProjectileMovementComp->ProjectileGravityScale = 0.f; // 직선운동
	ProjectileMovementComp->bRotationFollowsVelocity = true; // 추가 확인
	SetActorEnableCollision(true);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeTime();
}

void AProjectile::SetLifeTime()
{
	SetLifeSpan(1);
}
