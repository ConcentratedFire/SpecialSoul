// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemy.h"

#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/Components/EnemyFSMComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	FSMComponent = CreateDefaultSubobject<UEnemyFSMComponent>(TEXT("FSMComponent"));

	// 레벨 배치시 or 런타임 스폰시에 자동으로 Possess 되도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	 // 델리게이트 (중재자 역할)
	 // (FSM과 AnimInstance 간의 결합도를 낮춰주기 위함)
	 // FSM과 AnimInstance가 서로의 코드를 몰라도 되도록 !!
	if (FSMComponent)
	{
		FSMComponent->OnStateChange.AddDynamic(this, &ABaseEnemy::OnFSMStateChanged);
		FSMComponent->OnAttack.AddDynamic(this, &ABaseEnemy::HandleAttack);
		FSMComponent->OnDie.AddDynamic(this, &ABaseEnemy::HandleDie);
		
	}
}


void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABaseEnemy::OnFSMStateChanged(EEnemyState NewState)
{
	if (AnimInstance)
	{
		AnimInstance->CurrentState = NewState;
	}
}

void ABaseEnemy::HandleAttack()
{
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}
}

void ABaseEnemy::HandleDie()
{
	if (DieMontage)
	{
		PlayAnimMontage(DieMontage);
	}
}

