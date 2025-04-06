// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Components/EnemyFSMComponent.h"

// Sets default values for this component's properties
UEnemyFSMComponent::UEnemyFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
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
	}
}


void UEnemyFSMComponent::IdleTick(float DeltaTime) { }

void UEnemyFSMComponent::MoveTick(float DeltaTime){ }

void UEnemyFSMComponent::AttackTick(float DeltaTime) { }

void UEnemyFSMComponent::DieTick(float DeltaTime) { }
