// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AttackActors/CMiddleBossBullet.h"

#include "ObjectPool/CObjectPoolManager.h"

// Sets default values
ACMiddleBossBullet::ACMiddleBossBullet()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACMiddleBossBullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACMiddleBossBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTime <= MoveDuration)
	{
		float Alpha = CurrentTime / MoveDuration; // 0에서 1로 진행
		float EasedAlpha = easeInOutExpo(Alpha);
		FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, EasedAlpha);
		SetActorLocation(NewLocation);
		CurrentTime += DeltaTime;
	}
	else
		ObjectPoolManager->ReturnMiddleBossBullet(this);
}

void ACMiddleBossBullet::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);

	if (!bNewHidden)
	{
		StartLocation = GetActorLocation();
		TargetLocation = GetActorLocation() + GetActorForwardVector() * 500;
		CurrentTime = 0.f;
	}
}

float ACMiddleBossBullet::easeInOutExpo(float x)
{
	return x == 0? 0: x == 1? 1 : x < 0.5 ? FMath::Pow(2, 20 * x - 10) / 2 : (2 - FMath::Pow(2, -20 * x + 10)) / 2;
}
