// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Components/EnemyFSMComponent.h"

#include "EngineUtils.h"
#include "Player/CBasePlayer.h"

UEnemyFSMComponent::UEnemyFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}


void UEnemyFSMComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UEnemyFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (CurrentState)
	{
	case EEnemyState::Idle: IdleTick(DeltaTime); break;
	case EEnemyState::Move: MoveTick(DeltaTime); break;
	case EEnemyState::Attack: AttackTick(DeltaTime); break;
	case EEnemyState::Die: DieTick(DeltaTime); break;
	}
}


void UEnemyFSMComponent::SetState(EEnemyState NewState)
{
	if (CurrentState != NewState)
	{
		CurrentState = NewState;
		OnStateChange.Broadcast(CurrentState);
	}
}


void UEnemyFSMComponent::IdleTick(float DeltaTime) { }

void UEnemyFSMComponent::MoveTick(float DeltaTime) { }

void UEnemyFSMComponent::AttackTick(float DeltaTime) { }

void UEnemyFSMComponent::DieTick(float DeltaTime) { }
