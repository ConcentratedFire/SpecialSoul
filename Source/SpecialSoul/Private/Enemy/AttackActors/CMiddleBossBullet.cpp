// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AttackActors/CMiddleBossBullet.h"

#include "EngineUtils.h"
#include "ObjectPool/CObjectPoolManager.h"

// Sets default values
ACMiddleBossBullet::ACMiddleBossBullet()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
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

	if (OwnerMiddleBoss && !OwnerMiddleBoss->HasAuthority()) return;
	SRPC_MoveBullet();
}

void ACMiddleBossBullet::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);

	if (HasAuthority() && !bNewHidden)
	{
		StartLocation = GetActorLocation();
		TargetLocation = GetActorLocation() + GetActorForwardVector() * 500;
		CurrentTime = 0.f;

		for (TActorIterator<ACMiddleBoss> It(GetWorld(), ACMiddleBoss::StaticClass()); It; ++It)
		{
			OwnerMiddleBoss = *It;
			this->SetOwner(OwnerMiddleBoss);
		}
	}
}

float ACMiddleBossBullet::easeInOutExpo(float x)
{
	return x == 0 ? 0 : x == 1 ? 1 : x < 0.5 ? FMath::Pow(2, 20 * x - 10) / 2 : (2 - FMath::Pow(2, -20 * x + 10)) / 2;
}

void ACMiddleBossBullet::SRPC_MoveBullet_Implementation()
{
	if (CurrentTime <= MoveDuration)
	{
		float Alpha = CurrentTime / MoveDuration; // 0에서 1로 진행
		float EasedAlpha = easeInOutExpo(Alpha);
		FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, EasedAlpha);
		SetActorLocation(NewLocation);
		CurrentTime += GetWorld()->GetDeltaSeconds();
	}
	else
		ObjectPoolManager->ReturnMiddleBossBullet(this);
}
