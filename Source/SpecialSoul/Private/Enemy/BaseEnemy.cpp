// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemy.h"

#include "EngineUtils.h"
#include "Enemy/EnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CBasePlayer.h"

class ACBasePlayer;
// Sets default values
ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	

	// 레벨 배치시 or 런타임 스폰시에 자동으로 Possess 되도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	if (auto Anim = GetMesh()->GetAnimInstance())
	{
		AnimInstance = Cast<UEnemyAnimInstance>(Anim);
	}
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	StartFindingTarget();
}


void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABaseEnemy::StartFindingTarget()
{
	GetWorld()->GetTimerManager().SetTimer(FindTargetTimerHandle,
		this, &ABaseEnemy::FindTarget,
			FindTargetInterval, true
		);
}

void ABaseEnemy::FindTarget()
{
	Target = nullptr;
	
	TArray<AActor*> FoundPlayers;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACBasePlayer::StaticClass(), FoundPlayers);
	for (TActorIterator<ACBasePlayer> It(GetWorld(), ACBasePlayer::StaticClass()); It; ++It)
		FoundPlayers.Add(*It);

	AActor* ClosestPlayer = nullptr;
	float ClosestDist = FLT_MAX;

	for (AActor* Player : FoundPlayers)
	{
		float Dist = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
		if (Dist < ClosestDist)
		{
			ClosestPlayer = Player;
			ClosestDist = Dist;
		}
	}

	if (ClosestPlayer)
	{
		Target = ClosestPlayer;
	}
}

void ABaseEnemy::HandleAttack()
{
	PlayAnimMontage(AttackMontage);
}

void ABaseEnemy::HandleDie()
{
	PlayAnimMontage(DieMontage);
}

