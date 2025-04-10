// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Jinx/MinigunBullet.h"

#include "GameFramework/ProjectileMovementComponent.h"

AMinigunBullet::AMinigunBullet()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}

	MeshComp->SetCollisionProfileName("PlayerAttack");
}

void AMinigunBullet::BeginPlay()
{
	Super::BeginPlay();

	// 사정거리 / 속도 = 수명 (초 단위)
	float LifeSpan = AttackRange / ProjectileMovementComp->InitialSpeed;
	SetLifeSpan(LifeSpan); // 수명 설정
}
