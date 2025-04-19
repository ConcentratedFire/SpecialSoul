// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemy.h"

#include "EngineUtils.h"
#include "SpecialSoul.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/AI/CEnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ObjectPool/CObjectPoolManager.h"
#include "Player/CBasePlayer.h"

class ACBasePlayer;
// Sets default values
ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	ConstructorHelpers::FClassFinder<ACEnemyController> tempController(
		TEXT("/Script/Engine.Blueprint'/Game/Enemy/AI/BP_AIController.BP_AIController_C'"));
	if (tempController.Succeeded())
		AIControllerClass = tempController.Class;

	// 레벨 배치시 or 런타임 스폰시에 자동으로 Possess 되도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	if (ObjectPoolManager)
	{
		ObjectPoolManager->EnemyOutFromPool_Dele.AddUObject(this, &ABaseEnemy::OnMyControllerTickOn);
		ObjectPoolManager->EnemyGotoPool_Dele.AddUObject(this, &ABaseEnemy::OnMyControllerTickOff);
	}

	GetCapsuleComponent()->SetCollisionProfileName(FName("Enemy"));
	// GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// GetMesh()->SetCollisionProfileName(FName("Enemy"));
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	HP = MaxHP;

	MyController = Cast<ACEnemyController>(GetController());

	if (auto Anim = GetMesh()->GetAnimInstance())
	{
		AnimInstance = Cast<UEnemyAnimInstance>(Anim);
		AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseEnemy::OnMontageEnded);
	}

	StartFindingTarget();
}

void ABaseEnemy::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);

	if (!bNewHidden)
	{
		GetCharacterMovement()->GravityScale = 1;
		HP = MaxHP;
		MyController = Cast<ACEnemyController>(GetController());

		// FVector UpLocation = GetActorLocation();
		// float halfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		// UpLocation.Z += halfHeight;
		// SetActorLocation(UpLocation);

		StartFindingTarget();
	}
	else
		GetCharacterMovement()->GravityScale = 0;
}

void ABaseEnemy::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseEnemy, HP);
	DOREPLIFETIME(ABaseEnemy, bIsDead);
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
		if (HasAuthority())
			MyController->TargetPlayer = Target; // 클라 추가하면 크래시 발생
	}
}

void ABaseEnemy::HandleAttack()
{
	SRPC_PlayAttackAnim();
}

void ABaseEnemy::HandleDie()
{
	if (DieMontage) PlayAnimMontage(DieMontage);
}

void ABaseEnemy::ResetEnemy()
{
	bIsDead = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
}

void ABaseEnemy::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!HasAuthority()) return;
	// 공격 몽타주가 끝났다면
	if (Montage == AttackMontage)
	{
		MyController->bEndAttack = true;
	}
}

void ABaseEnemy::OnMyControllerTickOn()
{
	if (IsHidden()) return; // 보이지 않는것은 아직 풀에 있는것임

	ACEnemyController* EC = Cast<ACEnemyController>(GetOwner());
	EC->SetActorTickEnabled(true);
}

void ABaseEnemy::OnMyControllerTickOff()
{
	if (!IsHidden()) return; // 보이는 것은 아직 죽지 않은 것

	ACEnemyController* EC = Cast<ACEnemyController>(GetOwner());
	EC->SetActorTickEnabled(false);
}

bool ABaseEnemy::GetIsPlayerInRange(const float Range) const
{
	TArray<FHitResult> HitResults;
	FVector Start = GetActorLocation();
	FVector End = Start;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray<AActor*> ActorsToIgnore;

	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, MoveDistance, objectTypes,
	                                                             false,
	                                                             ActorsToIgnore,
	                                                             EDrawDebugTrace::None,
	                                                             HitResults,
	                                                             true);

	return bHit;
}

void ABaseEnemy::MyDamage(int32 DamageAmount)
{
	SRPC_Damage(DamageAmount);
}

void ABaseEnemy::SRPC_Damage_Implementation(int32 DamageAmount)
{
	HP -= DamageAmount;
	// LOG_S(Warning, TEXT("Name:%s, HP : %d"), *GetName(), HP);
	if (HP <= 0)
	{
		bIsDead = true;
		ACEnemyController* EC = Cast<ACEnemyController>(GetOwner());
		if (EC)
			EC->StopMovement();

		MRPC_Die();
		
		// 80% 확률 계산
		if (FMath::RandRange(1, 100) >= 20)
		{
			// 경험치 스폰
			FTransform SpawnTransform;
			FVector SpawnLocation = GetActorLocation();
			SpawnLocation.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			SpawnTransform.SetLocation(SpawnLocation);
			SpawnTransform.SetRotation(GetActorRotation().Quaternion());
			SpawnTransform.SetScale3D(FVector(.4));
			ObjectPoolManager->ExpSpawn(SpawnTransform);
		}
	}
}

void ABaseEnemy::MRPC_Die_Implementation()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	HandleDie();
}

void ABaseEnemy::SRPC_PlayAttackAnim_Implementation()
{
	MRPC_PlayAttackAnim();
}

void ABaseEnemy::MRPC_PlayAttackAnim_Implementation()
{
	PlayAnimMontage(AttackMontage);
}