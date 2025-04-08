// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemy.h"

#include "EngineUtils.h"
#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/AI/CEnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CBasePlayer.h"

class ACBasePlayer;
// Sets default values
ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	ConstructorHelpers::FClassFinder<ACEnemyController> tempController(
		TEXT("/Script/Engine.Blueprint'/Game/Enemy/AI/BP_AIController.BP_AIController_C'"));
	if (tempController.Succeeded())
		AIControllerClass = tempController.Class;

	// 레벨 배치시 or 런타임 스폰시에 자동으로 Possess 되도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	MyController = Cast<ACEnemyController>(GetController());

	if (auto Anim = GetMesh()->GetAnimInstance())
	{
		AnimInstance = Cast<UEnemyAnimInstance>(Anim);
		AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseEnemy::OnMontageEnded);
	}
	
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
		MyController->TargetPlayer = Target;
	}
}

void ABaseEnemy::HandleAttack()
{
	// PlayAnimMontage(AttackMontage);
	AnimInstance->Montage_Play(AttackMontage);
}

void ABaseEnemy::HandleDie()
{
	PlayAnimMontage(DieMontage);
}

void ABaseEnemy::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 공격 몽타주가 끝났다면
	if (Montage == AttackMontage)
	{
		MyController->bEndAttack = true;
	}
}