// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Components/EnemyFSMComponent.h"

#include "Enemy/BaseEnemy.h"
#include "Interface/PathFindingStrategy.h"

UEnemyFSMComponent::UEnemyFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UEnemyFSMComponent::InitPathFindingStrategy(TScriptInterface<IPathFindingStrategy> InPFStrategy)
{
	PFStrategy = InPFStrategy;
}


void UEnemyFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerEnemy = Cast<ABaseEnemy>(GetOwner());
}

void UEnemyFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!OwnerEnemy || !PFStrategy) return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ElapsedTime += DeltaTime;

	HP = OwnerEnemy->HP;
	if (HP <= 0)
		CurrentState = EEnemyState::Die;
	
	switch (CurrentState)
	{
	case EEnemyState::Move: MoveTick(DeltaTime); break;
	case EEnemyState::Attack: AttackTick(DeltaTime); break;
	case EEnemyState::Die: DieTick(DeltaTime); break;
	}
}

void UEnemyFSMComponent::SetState(EEnemyState NewState)
{
	CurrentState = NewState;
	ElapsedTime = 0.f;
	
	switch (CurrentState)
	{
	case EEnemyState::Attack:
		{
			ElapsedTime = AttackSpeed;
		} break;
	}
}

void UEnemyFSMComponent::MoveTick(float DeltaTime)
{
	PFStrategy->MoveTo(TargetLocation);
	PFStrategy->TickMove(DeltaTime); // PFStrategy를 이용한 이동처리

	 // Target과의 거리 체크
	if (FVector::Dist(TargetLocation, OwnerEnemy->GetActorLocation()) < AttackRange)
	{
		SetState(EEnemyState::Attack);
	}
}

void UEnemyFSMComponent::AttackTick(float DeltaTime)
{
	if (ElapsedTime >= AttackSpeed)
	{
		// 타겟으로 방향 회전
		FVector LookDir = (TargetLocation - OwnerEnemy->GetActorLocation()).GetSafeNormal();
		OwnerEnemy->SetActorRotation(LookDir.Rotation());
		
		OwnerEnemy->HandleAttack();
		ElapsedTime = 0.f;
		OwnerEnemy->bIsAttacking = true; // AnimNotify에서 false 처리
	}

	if (OwnerEnemy->bIsAttacking == false)
	{
		 // Target과의 거리 체크
		if (FVector::Dist(TargetLocation, OwnerEnemy->GetActorLocation()) >= AttackRange)
		{
			SetState(EEnemyState::Move);
		}
	}
}

void UEnemyFSMComponent::DieTick(float DeltaTime)
{
	if (HP > 0)
		CurrentState = EEnemyState::Move;
}
