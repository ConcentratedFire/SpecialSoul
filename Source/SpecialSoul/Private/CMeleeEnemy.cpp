// Fill out your copyright notice in the Description page of Project Settings.


#include "CMeleeEnemy.h"

#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/Components/EnemyFSMComponent.h"

class ACBasePlayer;

ACMeleeEnemy::ACMeleeEnemy()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(70.f);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -60));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeScale3D(FVector(0.008));
}

void ACMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();

	FSMComponent->Target;
	for (TActorIterator<ACBasePlayer> It(GetWorld(), ACBasePlayer::StaticClass()); It; ++It)
	{
		AActor* Actor = *It;
		OutActors.Add(Actor);
	}
	
	AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
}
