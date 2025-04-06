// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AttackActors/CTornado.h"

// Sets default values
ACTornado::ACTornado()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACTornado::BeginPlay()
{
	Super::BeginPlay();

	if (!TornadoDirection.IsZero())
	{
		SetActorRotation((TornadoDirection).Rotation());
	}
}

// Called every frame
void ACTornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * 100 * DeltaTime);
}
